void initializeFollowFocusMode(void){
	// inPointIsSet=false;
	// outPointIsSet=false;
	enableDriver();
	
	configureMicrosteping(16);

	// enableTimer1InterruptForVideoProMode();

	// Inicializamos los focus points
	for (int i=0;i<6;i++) {
		focusPoints[i].isDefined=false;
		focusPoints[i].steps = 0;
		focusPoints[i].durationTens=0;
		focusPoints[i].durationUnits=0;
		focusPoints[i].durationTenths=0;
	}

}

/* Funcion a la que se llama una vez pulsado SET en un punto y antes de entrar a la definicion del punto */
void pointDefinitionWillCall(void){
	currentPointBeingDefined=menuItem;

	steps=0;
	stepsPlus=0;
	stepsMinus=0;
	stepsToMove=0;

	// Cargamos los valores previos de la duración
	selectedDurationSecondsTens = focusPoints [currentPointBeingDefined].durationTens;
	selectedDurationSecondsUnits = focusPoints [currentPointBeingDefined].durationUnits;
	selectedDurationSecondsTenths = focusPoints [currentPointBeingDefined].durationTenths;

	enableTimer1InterruptForVideoProMode();
}

/*
Se llama cuando hemos terminado de definir la posicion del punto.
Comprobamos que el numero de pasos no haya sido 0
*/
bool pointPositionIsDefined (void) {
	// Salimos de la función si no nos hemos movido nada, salvo que estemos definiendo el primer punto
	if (currentPointBeingDefined != 0)
		if (stepsPlus==stepsMinus) return false;

	currentPosition = currentPointBeingDefined;
	return true;
}

/* 
Función a la que se llama una vez pulsado OK en la definición del punto
Se debe comprobar que todos los datos están correctamente definidos
Devuelve 1 si todo OK, 0 si NOK
*/
bool pointIsCompletelyDefined(void){
	focusPoints [currentPointBeingDefined].durationTens = selectedDurationSecondsTens;
	focusPoints [currentPointBeingDefined].durationUnits = selectedDurationSecondsUnits;
	focusPoints [currentPointBeingDefined].durationTenths = selectedDurationSecondsTenths;
	focusPoints [currentPointBeingDefined].isDefined = true;
	// Añadimos los pasos ya que puede que estemos entrando por primera vez o que estemos editando un punto.
	focusPoints [currentPointBeingDefined].steps += stepsPlus-stepsMinus;

	if (currentPointBeingDefined==0) { // Estamos editando el primer punto (A)
		focusPoints [currentPointBeingDefined].steps = 0;

		// ATENTCION: Si "modificamos" el primer punto, debemos cambiar los restantes.
	}

	return true;
}


void pointIsBeingDefinedFollowFocusMode(void) {
	// Creamos condición que nunca llegará para un movimiento continuo
	steps=0;
	stepCounter=2147483647;
	//if (lastMenuItem==menuItem) return;
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


unsigned char numFocusPointsDefined (void){
	for (unsigned char i = 0; i<6; i++) {
		if (focusPoints[i].isDefined==false)
			return i;
	}
	return 6;
}



char modeFollowFocusWillMoveToStart (void) {
	// Salimos si ya estamos en el principio
	if (currentPosition == 0)
		return 0;


	// Calculamos pasos a movernos hasta el inicio y los asignamos al contador de pasos del ISR
	stepCounter=0;
	for (int i=0;i<=currentPosition;i++) {
		stepCounter += focusPoints[i].steps; 
	}

	// Cambiamos el signo para ir hacia "atras"
	stepCounter *= -1;

	if (stepCounter>0) {
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
	}
	else {
		digitalWrite (A4988dirPin,HIGH);
		direction=HIGH;
	}

	// Eliminamos el signo
	stepCounter= abs(stepCounter);

	// Ajustamos una velocidad por defecto
	currentStepperSpeed = 30;

	// Iniciamos el contador de pasos del ISR a 0
	steps=0;
	
	// Movimiento sin aceleracion
	flag_ISR_acel=0;		

	_debug ("stepCounter: "); _debugln (stepCounter);

	// Deshabilitamos encoder
	disableEncoder();
	
	enableTimer1InterruptForVideoProMode();

	TIMSK1 |= (1 << OCIE1A);	// Comenzamos a movernos

	return 1;

}

/*
Decide si podemos mover de un punto a otro o si por el contrario solo podemos ir al inicio

*/
bool canMovePointToPoint (void) {
	if (inLimbo || currentPosition == numFocusPointsDefined()-1) 
		return false;
	else
		return true;
}


void modeFollowFocusWillRun (void) {
	// Calculamos pasos a movernos hasta el siguiente punto y los asignamos al contador de pasos del ISR
	
	stepCounter = focusPoints[currentPosition+1].steps;
	
	if (stepCounter>0) {
		digitalWrite (A4988dirPin,LOW);
		direction=LOW;
	}
	else {
		digitalWrite (A4988dirPin,HIGH);
		direction=HIGH;
	}
	
	// Eliminamos el signo
	stepCounter= abs(stepCounter);

	_debug ("stepCounter: "); _debugln (stepCounter);
	
	// Calculamos la velocidad del tramo
	selectedDuration = focusPoints[currentPosition+1].durationTens * 100;
	selectedDuration += focusPoints[currentPosition+1].durationUnits * 10;
	selectedDuration += focusPoints[currentPosition+1].durationTenths;

	float s = abs((float)stepCounter) / selectedDuration;
	speedToMoveHz = s * 10;	// Estamos tomando todas las duración * 10


		
	float ss = (float)(((float)(20000000/speedToMoveHz)-64)/64);
	speedToMove = round (ss);	

	_debug ("speedToMoveHz: "); _debugln (speedToMoveHz);
	_debug ("selectedDuration: "); _debugln (selectedDuration);	
	_debug ("stepCounter: "); _debugln (stepCounter);
	_debug ("ss: "); _debugln (ss);
	
	currentStepperSpeed = speedToMove;

	//currentStepperSpeed = 30;

	// Iniciamos el contador de pasos del ISR a 0
	steps=0;
	
	// Movimiento sin aceleracion
	flag_ISR_acel=0;		

	// Deshabilitamos encoder
	disableEncoder();
	
	enableTimer1InterruptForVideoProMode();

	TIMSK1 |= (1 << OCIE1A);	// Comenzamos a movernos

}

void followFocusModeIsRunning (void) {
	//_debugln ("followFocusModeIsRunning");
	if (steps >= stepCounter) { // Hemos llegado al final del movimiento
		if (STATE == SCN10S02) {	// Nos estábamos moviendo hasta el inicio
			currentPosition = 0;	// Estamos en el punto de inicio
			STATE = SCN10S01;	
			inLimbo=false;
		}

		if (STATE == SCN10S03) { 	//Movimiento de punto a punto
			currentPosition ++;		// Nos hemos movido hasta el siguiente punto
			STATE = SCN10S01;
			inLimbo=false;
		} 
		_debug ("StopFF.currentPosition: "); _debugln (currentPosition);	
		enableEncoder();
		
	}
	
}



























