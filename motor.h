
void enableDriver (void) {
	digitalWrite (A4988enablePin, LOW);
}

void disableDriver (void) {
	digitalWrite (A4988enablePin, HIGH);
}

// unsigned int convertPWM2TimerRegister (int s) {
// 	// Vmin y Vmax es distinta según el modo en el que estemos Video o TLapse
// 	if (STATE==RVM) 
// 		//return map (s, 0,255,minVideoStepperFreqOCR1A, maxVideoStepperFreqOCR1A);	
// 		return s;
// 	else
// 		return map (s, 0,255,minStepperFreqOCR1A, maxStepperFreqOCR1A);
// }

// void motorWillMove(void){
	
// 	// Empezamos a generar la onda cuadrada
// 	//enableTimer1Interrupt(convertPWM2TimerRegister(programParameters.speed)
// 	currentStepperSpeed = convertPWM2TimerRegister(programParameters.speed);
// 	//enableTimer1Interrupt();
// 	//enableTimer2Interrupt();	 
// 	enableTimer1InterruptForVideoMode();
// }

void processDirection(void) {
	/* Switch de dirección */
	if (digitalRead(dirSwitchPin)==HIGH)
		digitalWrite (A4988dirPin,LOW);
	else
		digitalWrite (A4988dirPin, HIGH);
}
/*
void motorWillStop(void) {
	// Deshabilitamos la interrupcion en Timer1
	disableTimer1Interrupt();
	//disableTimer2Interrupt();
}
*/
void moveMotorSteps (unsigned int s) {
	digitalWrite (A4988stepPin, LOW);
	for (unsigned int i=0; i<s; i++) {
		digitalWrite (A4988stepPin, HIGH);
		delay(5);
		digitalWrite (A4988stepPin, LOW);
		delay(5);
	}
}

/*
DRV8825
*******
MS0		MS1 	MS2
-------------------
Low 	Low		Low		Full step
High	Low		Low		Half step
Low		High	Low		1/4 step
High	High	Low		1/8 step
Low		Low		High	1/16 step
High	Low		High	1/32 step
Low		High	High	1/32 step
High	High	High	1/32 step

A4988
*****
MS1 	MS1 	MS2
-------------------
Low		Low		Low		Full step
High	Low		Low		Half step
Low		High	Low		Quarter step
High	High	Low		Eighth step
High	High	High	Sixteenth step
*/

void configureMicrosteping (char i) {
	switch (i) {
		case 1:
			digitalWrite (A4988ms1Pin, LOW);
			digitalWrite (A4988ms2Pin, LOW);
			digitalWrite (A4988ms3Pin, HIGH);
		break;
		
		case 2:
			digitalWrite (A4988ms1Pin, HIGH);
			digitalWrite (A4988ms2Pin, LOW);
			digitalWrite (A4988ms3Pin, LOW);
		break;

		case 4:
			digitalWrite (A4988ms1Pin, LOW);
			digitalWrite (A4988ms2Pin, HIGH);
			digitalWrite (A4988ms3Pin, LOW);
		break;

		case 8:
			digitalWrite (A4988ms1Pin, HIGH);
			digitalWrite (A4988ms2Pin, HIGH);
			digitalWrite (A4988ms3Pin, LOW);
		break;

		case 16:
			digitalWrite (A4988ms1Pin, HIGH);
			digitalWrite (A4988ms2Pin, HIGH);
			digitalWrite (A4988ms3Pin, HIGH);
		break;

	}
}