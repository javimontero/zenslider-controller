
/*
moveWithRamp
Atmel AVR446 datasheet

step:   Numero de pasos a movernos
accel:  Aceleración en rad/s2
decel:  Deceleración en rad/s2
speed:  Velocidad en Hz

Notas
-----
Algoritmo original AVR446

El prescaler está configurado en 8. El resto de la funcionalidad en 64.

Para poder usar el algoritmo la velocidad hay que multiplicarla por un
factor calculado empiricamente.

*/

void speed_cntr_Init_Timer1(void)
{
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B
  // Tells what part of speed ramp we are in.
  srd.run_state = STOP;
  // Timer/Counter 1 in mode 4 CTC (Not running).
  TCCR1B = (1<<WGM12);
  // Timer/Counter 1 Output Compare A Match Interrupt enable.
  TIMSK1 = (1<<OCIE1A);
}

void moveWithRamp(long step, unsigned int accel, unsigned int decel, unsigned long speed)
{
  //! Number of steps before we hit max speed.
  unsigned long max_s_lim;
  //! Number of steps before we must start deceleration (if accel does not hit max speed).
  unsigned long accel_lim;

  // Informamos al vector de interrupción que hay que usar aceleracion
  flag_ISR_acel=1;

  // Inicializamos interrupción
  speed_cntr_Init_Timer1();

  _debugln ("moveWithRamp");
  _debugln ("------------");
  _debug ("speed: "); _debugln (speed);
  
  speed=speed * (float)0.0245;
  //speed = (speed*2*3.14159)/3200;
  _debug ("speed: "); _debugln (speed);

  // If moving only 1 step.
  if(step == 1){
    // Move one step...
    srd.accel_count = -1;
    // ...in DECEL state.
    srd.run_state = DECEL;
    // Just a short delay so main() can act on 'running'.
    srd.step_delay = 1000;
    status.running = TRUE;
    OCR1A = 500;
    // Run Timer/Counter 1 with prescaler = 8.
    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
  }
  // Only move if number of steps to move is not zero.
  else if(step != 0){
    // Refer to documentation for detailed information about these calculations.

    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srd.min_delay = A_T_x100 / speed;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srd.step_delay = (T1_FREQ_148 * sqrt_Taylor(A_SQ / accel))/100;

    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    //max_s_lim = (long)speed*speed/(long)(((long)A_x20000*accel)/100);
    //max_s_lim = (long)speed*speed/(long)(((long)2*ALPHA*accel));
    max_s_lim = (unsigned long)(speed*speed) / (float)(2*ALPHA*accel*100);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    accel_lim = ((long)step*decel) / (accel+decel);
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srd.decel_val = accel_lim - step;
    }
    else{
      srd.decel_val = -((long)max_s_lim*accel)/decel;
    }
    // We must decelrate at least 1 step to stop.
    if(srd.decel_val == 0){
      srd.decel_val = -1;
    }

    // Find step to start decleration.
    srd.decel_start = step + srd.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srd.step_delay <= srd.min_delay){
      srd.step_delay = srd.min_delay;
      srd.run_state = RUN;
    }
    else{
      srd.run_state = ACCEL;
    }



    // Reset counter.
    srd.accel_count = 0;
    status.running = TRUE;
    OCR1A = 10;
    // Set Timer/Counter to divide clock by 8
    TCCR1B |= ((0<<CS12)|(1<<CS11)|(0<<CS10));
  }

  _debugln ("moveWithRamp");
  _debugln ("---------------");
  _debug ("step: "); _debugln (step);
  _debug ("speed: "); _debugln (speed);
  Serial.print("step_delay: "); Serial.println(srd.step_delay);
  Serial.print("decel_start: "); Serial.println(srd.decel_start);
  Serial.print("decel_val: "); Serial.println(srd. decel_val);
  Serial.print("min_delay: "); Serial.println(srd.min_delay);
  Serial.print("accel_count: "); Serial.println(srd.accel_count);
  Serial.print("max_s_lim : "); Serial.println(max_s_lim );
  Serial.print("accel_lim: "); Serial.println(accel_lim);
  //snprintf(buffer,sizeof(buffer),"%.15f\n",ALPHA);    

  _debug ("ALPHA: "); _debugln (ALPHA);
  _debug ("A_T_x100: "); _debugln (A_T_x100);
  _debug ("T1_FREQ_148: "); _debugln (T1_FREQ_148);
  _debug ("A_SQ: "); _debugln (A_SQ);
}




ISR(TIMER1_COMPA_vect) {
  if (flag_ISR_acel) {
    // Holds next delay period.
    static unsigned long new_step_delay;
    // Remember the last step delay used when accelrating.
    static unsigned long last_accel_delay;
    // Counting steps when moving.
    static unsigned long step_count = 0;
    // Keep track of remainder from new_step-delay calculation to incrase accurancy
    static unsigned int rest = 0;
    //static unsigned int r_step_count=0;

    OCR1A = srd.step_delay;
    //d[step_count]=srd.step_delay;
    //debugOCR1A[step_count]=OCR1A;
    //debug
   
    //d[step_count]=srd.step_delay;
    // r_step_count++;
    //r_step_count++;
    //debug

    /*
     Aunque la variable steps la utilizamos en el modo sin aceleración,
    necesitamos actualizarla para volver al punto de inicio en el 
    caso de cancelar el modo VideoPro.
    */
    steps ++;

    switch(srd.run_state) {
      case STOP:
        step_count = 0;
        rest = 0;
        // Stop Timer/Counter 1.
        TCCR1B &= ~((1<<CS12)|(1<<CS11)|(1<<CS10));
        status.running = FALSE;
        break;

      case ACCEL:

        //sm_driver_StepCounter(srd.dir);
        //sm_driver_StepCounter(srd.dir);
        digitalWrite (A4988stepPin,!digitalRead(A4988stepPin));

        step_count++;
        srd.accel_count++;
        new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
        rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
        // Check if we should start decelration.
        if(step_count >= srd.decel_start) {
          srd.accel_count = srd.decel_val;
          srd.run_state = DECEL;
        }
        // Check if we hitted max speed.
        else if(new_step_delay <= srd.min_delay) {
          last_accel_delay = new_step_delay;
          new_step_delay = srd.min_delay;
          rest = 0;
          srd.run_state = RUN;
        }
        break;

      case RUN:

        digitalWrite (A4988stepPin,!digitalRead(A4988stepPin));
        step_count++;
        new_step_delay = srd.min_delay;
        // Chech if we should start decelration.
        if(step_count >= srd.decel_start) {
          srd.accel_count = srd.decel_val;
          // Start decelaration with same delay as accel ended with.
          new_step_delay = last_accel_delay;
          srd.run_state = DECEL;
        }
   
        break;

      case DECEL:
        //sm_driver_StepCounter(srd.dir);
      digitalWrite (A4988stepPin,!digitalRead(A4988stepPin));
        step_count++;
        srd.accel_count++;
        new_step_delay = srd.step_delay - (((2 * (long)srd.step_delay) + rest)/(4 * srd.accel_count + 1));
        rest = ((2 * (long)srd.step_delay)+rest)%(4 * srd.accel_count + 1);
        // Check if we at last step
        if (srd.accel_count >= 0) {
          srd.run_state = STOP;
  
        }
        break;
    }
    srd.step_delay = new_step_delay;
  }
  else {
    /*   Modo SIN aceleración   */ 
    OCR1A=currentStepperSpeed;    
    steps++;
    if (direction==LOW) stepsPlus++;
    else stepsMinus++;

    digitalWrite (A4988stepPin, !digitalRead(A4988stepPin)); //Invertimos onda
    
    // Deshabilitamos la interrupción (paramos motor)
    if (steps>=stepCounter) TIMSK1 &= ~(1<<OCIE1A); 


  }
}

// ISR(TIMER1_COMPA_vect) {
//   /*   Modo con aceleración   */ 
//   if (flag_ISR_acel) {
//     OCR1A=currentStepperSpeed;   

//     if (steps < aRamp.ac) {
//       currentStepperSpeed = currentStepperSpeed / 1.2;
//       if (currentStepperSpeed > aRamp.s) currentStepperSpeed=aRamp.s;
//     }
    
//     if (steps > aRamp.dc) {
//       currentStepperSpeed = currentStepperSpeed * 1.2;
//       //if (currentStepperSpeed < aRamp.s) currentStepperSpeed=aRamp.s;
//     }

//     steps++;
//     if (direction==LOW) stepsPlus++;
//     else stepsMinus++;

//     digitalWrite (A4988stepPin, !digitalRead(A4988stepPin)); //Invertimos onda
    
//     // Deshabilitamos la interrupción (paramos motor)
//     if (steps>=stepCounter) TIMSK1 &= ~(1<<OCIE1A); 
//   }
//   else {
//     /*   Modo SIN aceleración   */ 
//     OCR1A=currentStepperSpeed;    
//     steps++;
//     if (direction==LOW) stepsPlus++;
//     else stepsMinus++;
//     digitalWrite (A4988stepPin, !digitalRead(A4988stepPin)); //Invertimos onda
    
//     // Deshabilitamos la interrupción (paramos motor)
//     if (steps>=stepCounter) TIMSK1 &= ~(1<<OCIE1A); 
//   }

// }

unsigned long sqrt_Taylor(unsigned long x)
{
  register unsigned long xr;  // result register
  register unsigned long q2;  // scan-bit register
  register unsigned char f;   // flag (one bit)

  xr = 0;                     // clear result
  q2 = 0x40000000L;           // higuest possible result bit
  do
  {
    if((xr + q2) <= x)
    {
      x -= xr + q2;
      f = 1;                  // set flag
    }
    else{
      f = 0;                  // clear flag
    }
    xr >>= 1;
    if(f){
      xr += q2;               // test flag
    }
  } while(q2 >>= 2);          // shift twice
  if(xr < x){
    return xr +1;             // add for rounding
  }
  else{
    return xr;
  }
}


