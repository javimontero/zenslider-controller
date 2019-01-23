void pointDefineWillRun (void) {
	enableTimer1InterruptForVideoProMode();
	steps=0;
	stepsPlus=0;
	stepsMinus=0;
}

void defineDurationInterval (void) {
	_debugln ("defineDurationInterval");
	_debugln ("----------------------");
	_debug ("stepsToMove: "); _debugln (stepsToMove);

	maxDuration = abs(stepsToMove) / minSpeedVideoPro;
	minDuration = (int)ceil ((float)abs(stepsToMove) / maxSpeedVideoPro);
	// Hacemos que la duración mínima sea de 1 segundo
	if (minDuration<=1) minDuration=1;
	_debug ("maxDuration: "); _debugln (maxDuration);
	_debug ("minDuration: "); _debugln (minDuration);
	durationIntervalIsSet = true;

	// Definimos la duración minima por defecto
	if (selectedDuration < minDuration || selectedDuration > maxDuration)
		selectedDuration=minDuration;
}

void inPointIsDefined(void) {
	// Deshabilitamos la interrupción (paramos motor)
	TIMSK1 &= ~(1<<OCIE1A);
	
	lastPointSet=IN;
	long s= stepsPlus - stepsMinus;
	_debugln ("");
	_debugln ("inPointIsDefined");
	_debugln ("----------------");
	_debug ("stepsPlus: "); _debugln (stepsPlus);
	_debug ("stepsMinus: "); _debugln (stepsMinus);

	stepsPlus=0;
	stepsMinus=0;
	

	/*  Calculo de la distancia que separa a los puntos in y out 
	    Tomamos siempre como referencia in
	    La distancia que los separa será stepsToMove (+ o -)
		
		s es la distancia que se ha movido

	*/
	// if (inPointIsSet) {
	// 	/* Nuevo calculo */
	// 	if (currentPosition == OUT) {
	// 		stepsToMove=-s;
	// 	}
	// 	if (currentPosition == IN)
	// 		stepsToMove=stepsToMove-s;
	// }

	if (inPointIsSet && outPointIsSet)
		stepsToMove += s;
	
	if (!inPointIsSet && !outPointIsSet)
		stepsToMove = 0;

	if (inPointIsSet && !outPointIsSet)
		stepsToMove = 0;	

	if (!inPointIsSet && outPointIsSet)
		stepsToMove = -s;

	inPointIsSet = true;
	currentPosition=IN;

	// Inicializamos la variable contadora de pasos del timer.
	steps=0;

	_debug ("s: "); _debugln (s);
	_debug ("stepsToMove: "); _debugln (stepsToMove);
}

void outPointIsDefined(void) {
	// Deshabilitamos la interrupción (paramos motor)
	TIMSK1 &= ~(1<<OCIE1A);
	
	
	long s= stepsPlus - stepsMinus;
	// _debugln ("");
	// _debugln ("outPointIsDefined");
	// _debugln ("----------------");
	// _debug ("stepsPlus: "); _debugln (stepsPlus);
	// _debug ("stepsMinus: "); _debugln (stepsMinus);

	stepsPlus=0;
	stepsMinus=0;

	/*  Calculo de la distancia que separa a los puntos in y out 
	    Tomamos siempre como referencia in
	    La distancia que los separa será stepsToMove (+ o -)
		
		s es la distancia que se ha movido

	*/ 

	/* Nuevo calculo*/
	// if (outPointIsSet) {
	// 	if (currentPosition == OUT) 
	// 		stepsToMove+=s;
		
	// 	if (currentPosition == IN)
	// 		stepsToMove=s;
	// }
	// else  stepsToMove = s;

	// _debug ("s: "); _debugln (s);
	// _debug ("stepsToMove: "); _debugln (stepsToMove);

	if (inPointIsSet && outPointIsSet)
		stepsToMove += s;
	
	if (!inPointIsSet && !outPointIsSet)
		stepsToMove = 0;

	if (inPointIsSet && !outPointIsSet)
		stepsToMove = s;	

	if (!inPointIsSet && outPointIsSet)
		stepsToMove = 0;

	
	currentPosition=OUT;	
	lastPointSet=OUT;
	outPointIsSet = true;

	// Inicializamos la variable contadora de pasos del timer.
	steps=0;
}


void motorWillMoveToStartPosition(void) {

	// Estamos en el extremo OUT
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
	
	/*
	Estamos entre el extremo IN y el OUT. Esto es debido a que o
	1- Hemos parado el viaje hasta el inicio
	2- Hemos parado el programa Video Pro
	2- Hemos parado el programa ping-pong 
	*/
	if (currentPosition == FROM_IN_TO_OUT) { // Cambiamos la dirección
		if (stepsToMove<0) {
			direction=LOW;
			digitalWrite (A4988dirPin,LOW);
		}
		else {
			direction=HIGH;
			digitalWrite (A4988dirPin,HIGH);
		}
		// Ajustamos los pasos a moverse hasta el punto de entrada
		steps=stepCounter-steps;
	}
	if (currentPosition == FROM_OUT_TO_IN) { // Cambiamos la dirección
		if (stepsToMove<0) {
			direction=HIGH;
			digitalWrite (A4988dirPin,HIGH);
		}
		else {
			direction=LOW;
			digitalWrite (A4988dirPin,LOW);
		}
		// Ajustamos los pasos a moverse hasta el punto de entrada
		steps=stepCounter-steps;
	}
	
	// No inicializamos steps=0 porque puede que estemos parados a mitado de viaje
	
	//stepCounter = abs(stepsToMove);

	
	// Deshabilitamos encoder
	disableEncoder();

	// Iniciamos con velocidad
	currentStepperSpeed = speedGoToStart;
	
	// Modo sin aceleración
	// _debugln ("motorWillMoveToStartPosition");
	// _debug ("stepCounter: "); _debugln (stepCounter);
	// _debug ("currentPosition: "); _debugln (currentPosition);
	// _debug ("currentStepperSpeed: "); _debugln (currentStepperSpeed);
	
	
	flag_ISR_acel=0;
	enableTimer1InterruptForVideoProMode();

	TIMSK1 |= (1 << OCIE1A);

	currentPosition = FROM_OUT_TO_IN;
}

void motorIsMovingToStartPosition (void) {
	// _debug ("motorIsMovingToStartPosition steps stepCounter"); 
	// _debug (steps); _debug (" "); _debugln (stepCounter);
	_debug (" "); _debugln (OCR1A);

	if (steps>=stepCounter) {
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	if (STATE == SCN07S09)
	  		STATE = SCN07S01;
	  	else
	  		STATE=SCN06S01;
	  	
	  	currentPosition=IN;
	  	steps=0;
	  	// Habilitamos encoder
		enableEncoder();
	}
}