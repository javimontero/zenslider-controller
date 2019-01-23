void initializeVideoProMode(void){
	inPointIsSet=false;
	outPointIsSet=false;
	enableDriver();
	steps=0;
	stepsPlus=0;
	stepsMinus=0;
	stepsToMove=0;
}



void pointIsBeingDefinedVideoProMode(void) {
	// Creamos condición que nunca llegará para un movimiento continuo
	steps=0;
	stepCounter=2147483647;
	if (lastMenuItem==menuItem) return;
	switch (menuItem) {
		case -3:
		//digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedPPP_VP;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case -2:
		//digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedPP_VP;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case -1:
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedP_VP;
		TIMSK1 |= (1 << OCIE1A);
		break;

		case 0:
		// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);
		break;

		case 1:
		digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedP_VP;
		TIMSK1 |= (1 << OCIE1A);
		break;

		case 2:
		//digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedPP_VP;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case 3:
		// digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedPPP_VP;
		// TIMSK1 |= (1 << OCIE1A);

		break;


	}
}

void calculateVideoProParameters (void) {
	if (inPointIsSet && outPointIsSet) {
		// Calculamos speedToMove

		//int s = abs(stepsToMove) / selectedDuration;		// Frecuencia en Hz de la velocidad
		speedToMoveHz = abs(stepsToMove) / selectedDuration;		// Velocidad en Frecuencia en Hz de la velocidad
		
		float ss = (float)(((float)(20000000/speedToMoveHz)-64)/64);
		speedToMove = round (ss);									// Velocidad en terminos de OCR1A
		

		stepCounter = abs (stepsToMove);

		// _debugln ("");
		// _debugln("calculateVideoProParameters");
		 _debug ("Duration "); _debugln (selectedDuration);
		// _debug ("stepsToMove "); _debugln (stepsToMove);
		// _debug ("speedToMove (Hz): "); _debugln (s);
		//

		 _debug ("speedToMove ss (OCR): "); _debugln (ss);
		// _debug ("speedToMove (OCR): "); _debugln (speedToMove);
	}
}


void videoProModeWillRun (void) {
	// _debugln ("");
	// _debugln("videoProModeWillRun");
	// _debug ("Duration "); _debugln (selectedDuration);
	// _debug ("stepsToMove "); _debugln (stepsToMove);
	// _debug ("speedToMove (OCR): "); _debugln (speedToMove);
	// Definimos velocidad
	currentStepperSpeed = speedToMove;

	// Sentido de la marcha
	if (stepsToMove>0) {
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
	}
	else {
		digitalWrite (A4988dirPin,HIGH);
		direction=HIGH;
	}

	// 	steps=0;
 	stepCounter = abs (stepsToMove);

	// Deshabilitamos encoder
	disableEncoder();

	STATE=RVMP;
	//TIMSK1 |= (1 << OCIE1A);
	currentPosition = FROM_IN_TO_OUT;
	
	//TIMSK1 |= (1 << OCIE1A);
	
	_debugln ("videoProModeWillRun");
	_debugln ("-------------------");
	_debug ("stepCounter "); _debugln (stepCounter);
	_debug ("speedToMove Hz: "); _debugln (speedToMoveHz);
	_debug ("speedToMove rad/s: "); _debugln (speedToMove);


	moveWithRamp(stepCounter, 80, 80, speedToMoveHz);

	//moveWithRamp(stepCounter, 20, 20, speedToMoveHz);
}

void videoProModeIsRunning(void){
	_debug (srd.run_state); _debug (" "); _debugln (OCR1A);
	
	if (srd.run_state == STOP) {
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	STATE=SCN06S01;
	  	currentPosition=OUT;
	  	steps=0;

	  	// Habilitamos encoder
		enableEncoder();

	  	// Devolvemos el flag a estado No aceleración
        flag_ISR_acel=0;

        // Reconfiguramos el prescaler a 64
        TCCR1B |= (1 << CS10);
    	TCCR1B |= (1 << CS11);
	  	_debugln ("videoProModeIsRunning STOP");

	}
}

void pingPongModeWillRun (void) {
	
	currentStepperSpeed = speedToMove;

	// Sentido de la marcha

	if (currentPosition==OUT) {
		if (stepsToMove<0) { // IN a la derecha de OUT
			digitalWrite (A4988dirPin,LOW);
			direction =LOW;
		}
		else {
			digitalWrite (A4988dirPin,HIGH);
			direction = HIGH;	
		}
		//currentPosition = FROM_OUT_TO_IN;
	}
	
	if (currentPosition==IN) {
		if (stepsToMove<0) { // IN a la derecha de OUT
			digitalWrite (A4988dirPin,HIGH);
			direction =HIGH;
		}
		else {
			digitalWrite (A4988dirPin,LOW);
			direction = LOW;	
		}
		//currentPosition = FROM_OUT_TO_IN;
	}


	// 	steps=0;
 	stepCounter = abs (stepsToMove);

	// Deshabilitamos encoder
	disableEncoder();

	STATE=SCN06S06;

	moveWithRamp(stepCounter, 80, 80, speedToMoveHz);

}
void pingPongModeIsRunning(void){
	//_debug (srd.run_state); _debug (" "); _debugln (OCR1A);
	
	if (srd.run_state == STOP) { // final del recorrido
		//// Deshabilitamos la interrupción (paramos motor)
	  	//TIMSK1 &= ~(1<<OCIE1A);	
	  	//STATE=SCN06S01;
	  	
	  	//steps=0;

	  	// Invertimos el sentido de la marcha
	  	if (currentPosition == FROM_IN_TO_OUT) currentPosition = FROM_OUT_TO_IN;
	  	digitalWrite (A4988dirPin,!digitalRead(A4988dirPin));
	  	
	  	// Volvemos a empezar el movimiento en el sentido contrario
	  	stepCounter = abs (stepsToMove);
	  	moveWithRamp(stepCounter, 80, 80, speedToMoveHz);


	  	// Habilitamos encoder
		//enableEncoder();

	  	// Devolvemos el flag a estado No aceleración
        //flag_ISR_acel=0;

        // Reconfiguramos el prescaler a 64
        //TCCR1B |= (1 << CS10);
    	//TCCR1B |= (1 << CS11);
	  	//_debugln ("videoProModeIsRunning STOP");

	}
}

void pingPongModeWillStop (void) {
	
	// Paramos motor
	flag_ISR_acel=0;
	TIMSK1 &= ~(1<<OCIE1A);	

	// Reseteamos el modo completamente para tener que empezar de nuevo
	initializeVideoProMode();
}