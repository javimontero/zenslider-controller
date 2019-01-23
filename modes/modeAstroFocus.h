void initializeAstroFocusMode(void){
	inPointIsSet=false;
	outPointIsSet=false;
	enableDriver();
	steps=0;
	stepsPlus=0;
	stepsMinus=0;
	stepsToMove=0;
}


void calculateAstroFocusParameters (void) {
	if (inPointIsSet && outPointIsSet) {

		// Calculamos la duración en segundos
		unsigned int duration = selectedDurationSecondsUnits;
		duration += selectedDurationSecondsTens * 10;
		duration += selectedDurationMinutesUnits * 60;
		duration += selectedDurationMinutesTens * 60 * 10;
		duration += selectedDurationHours * 3600;

		// dividimos la duracion entre 2 en caso de ser ida y vuelta
		if (astroRoundTrip != 0) duration = (int) (duration/2);

		//speedToMoveHz = abs(stepsToMove) / duration;		// Velocidad en Frecuencia en Hz de la velocidad
		speedToMoveHz = (float)abs(stepsToMove) / duration;		// Velocidad en Frecuencia en Hz de la velocidad

		period = 1000/speedToMoveHz;	// Periodo en ms		
		//float ss = (float)(((float)(20000000/speedToMoveHz)-64)/64);
		//speedToMove = round (ss);	// Velocidad en terminos de OCR1A

		stepCounter = abs (stepsToMove);


	}
}


void astroFocusModeWillRun (void) {

	// Comprobaciones previas 
	// Se han definido los puntos de inicio y final
	// La duracion no es 0

	if (!inPointIsSet || !outPointIsSet) return;

	if (selectedDurationSecondsUnits==0 &&
		selectedDurationSecondsTens == 0 &&
		selectedDurationMinutesUnits == 0 &&
		selectedDurationMinutesTens == 0 &&
		selectedDurationHours == 0 ) 
			return;

	// Sentido de la marcha
	if (stepsToMove>0) {
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
	}
	else {
		digitalWrite (A4988dirPin,HIGH);
		direction=HIGH;
	}

	configureMicrosteping (16);

	steps=0;	
	disableEncoder();
	flag_ISR_acel=0;	// Movimiento sin aceleración

    // Configuramos el prescaler del timer dependiendo de la velocidad
	if (speedToMoveHz >= 1.10) {
		useTimer = true;
		// Definimos la velocidad dentro de la funcion enableTimer1InterruptForAstroFocusMode()
		enableTimer1InterruptForAstroFocusMode ();		
		
		// Enable interrupt
    	TIMSK1 |= (1 << OCIE1A);
	} else {
		// No podemos usar Timer
		TIMSK1 &= ~(1<<OCIE1A);
		useTimer= false;
		timerShootInterval=millis();	// Inicializamos contador	
	}
	
	STATE=RAFM;

}

void astroFocusModeIsRunning(void){
	//_debugln (OCR1A);
	_debug ("astroRoundTrip "); _debugln (astroRoundTrip);
	
	if (useTimer) { // Estamos usando Timer para generar pulso motor
		_debugln ("usetimer");
		//_debug (" "); _debugln (OCR1A);
		//_debug (" "); _debugln (TCCR1B);
		if (steps>=stepCounter) { //final del trayecto
			// Comprobamos si tenemos que dar la vuelta (modo ida y vuelta)
			if (astroRoundTrip == 0) {
				// Hemos llegado al final
				TIMSK1 &= ~(1<<OCIE1A);	
			  	STATE=SCN07S01;
			  	currentPosition=OUT;
			  	steps=0;

			  	// Habilitamos encoder
				enableEncoder();
			}
			else {
				// Tenemos que dar la vuelta
				if (astroRoundTrip == 2) astroRoundTrip =1;
				else astroRoundTrip = 2;
				//astroRoundTrip=2;
				digitalWrite (A4988dirPin,!digitalRead(A4988dirPin));
				steps=0;
				TIMSK1 |= (1 << OCIE1A);

			}	
		}		
	} else {
		_debugln ("no usetimer");
		//_debug ("period: "); _debugln (period);
		//_debug ("steps: "); _debugln (stepCounter);

		// Generamos onda de forma manual, sin usar timer.
		if (steps<=stepCounter) {
			if (millis() - timerShootInterval > period) {
				steps++;
				digitalWrite (A4988stepPin, !digitalRead(A4988stepPin)); //Invertimos onda
				timerShootInterval=millis();
				_debug ("steps: "); _debugln (steps);
			}
		}
		else { 
			// Comprobamos si tenemos que dar la vuelta o hemos finalizado el trayecto
			//if (astroRoundTrip == 0 || astroRoundTrip == 2) {
			if (astroRoundTrip == 0) {
				steps=0;
				STATE = SCN07S01;
				enableEncoder();
			}
			else {
				// HAcemos del modo Round Trip un blucle infinito (ping-pong)
				// Tenemos que dar la vuelta
				if (astroRoundTrip == 2) astroRoundTrip =1;
				else astroRoundTrip = 2;
				digitalWrite (A4988dirPin,!digitalRead(A4988dirPin));
				steps=0;
			}
		}
	}

}

void pointIsBeingDefinedAstroFocusMode(void) {
	// Creamos condición que nunca llegará para un movimiento continuo
	steps=0;
	stepCounter=2147483647;
	if (lastMenuItem==menuItem) return;
	switch (menuItem) {
		case -3:
		//digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedPPP_AF;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case -2:
		//digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedPP_AF;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case -1:
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
		currentStepperSpeed=speedP_AF;
		TIMSK1 |= (1 << OCIE1A);
		break;

		case 0:
		// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);
		break;

		case 1:
		digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedP_AF;
		TIMSK1 |= (1 << OCIE1A);
		break;

		case 2:
		//digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedPP_AF;
		//TIMSK1 |= (1 << OCIE1A);
		break;

		case 3:
		// digitalWrite (A4988dirPin, HIGH);
		direction=HIGH;
		currentStepperSpeed=speedPPP_AF;
		// TIMSK1 |= (1 << OCIE1A);

		break;


	}
}








































