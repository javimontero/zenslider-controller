void videoProgramWillRun (void) {
	STATE=RVM;                   
	PROGRAM_STATE=RVM01;
	
	// Actualizar los parametros del programa
	programParameters.speed=menuVideo.speed[menuItem];
	programParameters.intervalDuration=0;
	
	// Configuramos micropasos
	configureMicrosteping(16);
	
	enableDriver ();

	currentStepperSpeed=programParameters.speed;
	
	// Creamos condición que nunca llegará para un movimiento continuo
	steps=0;
	stepCounter=2147483647;

	//Arrancamos motor para modo Video
	enableTimer1InterruptForVideoMode();
}


void videoProgramIsRunning() {

	/*Evitamos overflow*/

	steps=0;
	stepsPlus=0;
	stepsMinus=0;
	

	switch (PROGRAM_STATE) {
		 /****** Estado 1. Parado ******/ 
		case RVM00:
		break;

		/****** Estado 2. Movimiento del motor ******/ 
		case RVM01:     
		
		// Actualizamos la velocidad cuando menuItem cambia (movimiento encoder)
		if (programParameters.speed!=menuVideo.speed[menuItem]) {
			programParameters.speed=menuVideo.speed[menuItem];
			currentStepperSpeed=programParameters.speed;
		}
		
		processDirection();
		
		break;
	}
}

void videoProgramWillStop (void) {
	disableTimer1Interrupt();
	//disableDriver();
	PROGRAM_STATE=RVM00;
}
