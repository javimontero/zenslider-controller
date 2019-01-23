
void enableTimer1Interrupt() { 
	//Serial.begin(115200);

	 // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 	// ********************************************************************************
 	// ********************************************************************************
    // Inicializar TCNTx a 0 para incializar el valor actual del timer
     TCNT1 = 0;
 	// ********************************************************************************
 	// ********************************************************************************
    // set compare match register to desired timer count:
    //OCR1A = currentStepperSpeed;
    OCR1A=currentStepperSpeed;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    //TCCR1B |= (1 << CS10);
    //TCCR1B |= (1 << CS12);
    // Set CS10 and CS12 bits for 64 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts:
    sei();
}


void enableTimer1InterruptForVideoMode(){

	//Serial.begin(115200);

	 // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 	// ********************************************************************************
 	// ********************************************************************************
    // Inicializar TCNTx a 0 para incializar el valor actual del timer
     TCNT1 = 0;
 	// ********************************************************************************
 	// ********************************************************************************
    // set compare match register to desired timer count:
    OCR1A = currentStepperSpeed;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    //El prescaler 1024 no nos da la suficiente granularidad para el modo video. Ver excel.

    // Set CS10 and CS12 bits for 64 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts:
    sei();
}

void enableTimerP256() {
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B

    TCNT1 = 0;

    OCR1A = currentStepperSpeed;
    // turn on CTC mode:
 
    TCCR1B |= (1 << WGM12);
 
    TCCR1B |= (0 << CS10);
    TCCR1B |= (0 << CS11);
    TCCR1B |= (1 << CS12);
 
    // enable global interrupts:
    sei();
}
void enableTimer1InterruptForAstroFocusMode () {
    float ss;


    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    
    // Inicializar TCNTx a 0 para incializar el valor actual del timer
    TCNT1 = 0;
    // set compare match register to desired timer count:
    
    //OCR1A = currentStepperSpeed;
    
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    
    // Configuramos el prescaler del timer dependiendo de la velocidad
    if (speedToMoveHz>=38) {
        // prescaler=8
        TCCR1B |= (0 << CS10);
        TCCR1B |= (1 << CS11);
        TCCR1B |= (0 << CS12);
        //ss = (float)(((float)(20000000/(4*speedToMoveHz))-8)/8);
        ss= (20000000 - (8*speedToMoveHz) ) / (8*speedToMoveHz);
        _debugln ("prescaler: 8");
    }

    if (speedToMoveHz < 38 && speedToMoveHz >= 4.70) {
        // prescaler = 64
        TCCR1B |= (1 << CS10);
        TCCR1B |= (1 << CS11);
        TCCR1B |= (0 << CS12);
        //ss = (float)(((float)(20000000/(4*speedToMoveHz))-64)/64);
        ss= (20000000 - (64*speedToMoveHz) ) / (64*speedToMoveHz);
        _debugln ("prescaler: 64");
    }

    if (speedToMoveHz < 4.70 && speedToMoveHz >= 1.10) {
        // prescaler = 256
        TCCR1B |= (0 << CS10);
        TCCR1B |= (0 << CS11);
        TCCR1B |= (1 << CS12);
        //ss = (float)(((float)(20000000/(4*speedToMoveHz))-256)/256);
        ss= (20000000 - (256*speedToMoveHz) ) / (256*speedToMoveHz);
        _debugln ("prescaler: 256");
    }
    
    // if (speedToMoveHz < 0.6 && speedToMoveHz >= 0.15) {
    //     // prescaler = 1024
    //     //TCCR1B |= (1 << CS10);
    //     TCCR1B |= (0 << CS10);
    //     TCCR1B |= (0 << CS11);
    //     TCCR1B |= (1 << CS12);
    //     //s<peedToMoveHz = (float)speedToMoveHz/2;
    //     // ss = (float)(((float)(20000000/(float)(speedToMoveHz))-1024)/1024);

    //     ss= (20000000 - (1024*2*speedToMoveHz) ) / (1024*2*speedToMoveHz);
    //     _debugln ("prescaler: 1024");
    // }

    currentStepperSpeed = round (ss);
    
    OCR1A = currentStepperSpeed;
    //OCR1A = 39200;

    sei();

    _debugln ("----");
    _debug ("speedToMoveHz: "); _debugln (speedToMoveHz);
    _debug ("OCR1A: "); _debugln (currentStepperSpeed);
    _debug ("TCCR1B: "); _debugln (TCCR1B);
}

void disableTimer1Interrupt (void) {
	//cli();
	//TCCR1A = 0;     // set entire TCCR1A register to 0
    //TCCR1B = 0;     // same for TCCR1B
  	//sei();
    TIMSK1 &= ~(1<<OCIE1A);
}


void enableTimer1InterruptForVideoProMode(){

    // Preparamos la interrupción pero no la habilitamos

     // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
    // ********************************************************************************
    // ********************************************************************************
    // Inicializar TCNTx a 0 para incializar el valor actual del timer
     TCNT1 = 0;
    // ********************************************************************************
    // ********************************************************************************
    // set compare match register to desired timer count:
    OCR1A = currentStepperSpeed;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 64 prescaler:

    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS11);

    // enable timer compare interrupt:
    //TIMSK1 |= (1 << OCIE1A);

    // enable global interrupts:
    sei();
}
















