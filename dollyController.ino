#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif


#define _MAINFILE
//#include <avr/eeprom.h>

#include "U8glib.h"

#include "global.h"


#include "dollyController.h"
#include "lcdstrings.h"
#include "motor.h"
#include "encoder.h"

#include "timer.h"
#include "EEPROM.h"

#include "acceleration/a_global.h"
#include "acceleration/accelerationRamp.h"

#include "modes/commonRoutines.h"
#include "modes/modeVideoPro.h"
#include "modes/modeVideo.h"
#include "modes/modeAstroFocus.h"
#include "modes/modeFollowFocus.h"

#include "render.h"
/***********************************************************************************/

void setup(void) {
	#ifdef DEBUG
	Serial.begin (57600);   
	#endif
	
	#ifdef DEBUG
	Serial.print ("Version: "); Serial.println (version);
	#endif

	u8gsetup();
	
  	pinMode (selectButtonPin, INPUT);  
	pinMode (dirSwitchPin, INPUT);
	pinMode (encoderSelectPin, INPUT);

	// Pines del motor driver
	
	pinMode (A4988dirPin,OUTPUT);
	pinMode (A4988ms1Pin,OUTPUT);
	pinMode (A4988ms2Pin,OUTPUT);
	pinMode (A4988ms3Pin,OUTPUT);
	pinMode (A4988enablePin,OUTPUT);
	pinMode (A4988stepPin,OUTPUT);
	digitalWrite (A4988enablePin,HIGH);

	/* Pololu A4988 microstepping 
	ms1 ms2	ms3
	LOW  LOW  HIGH 	Full
	HIGH LOW  LOW	Half
	LOW  HIGH LOW	Quarter1
	HIGH HIGH LOW	Eight
	HIGH HIGH HIGH  Sixteenth
	*/	

	// Microstepping
	configureMicrosteping(16);

                                     
	pinMode (trigger1, OUTPUT);
	pinMode (trigger2, OUTPUT);
	pinMode (testLed1, OUTPUT);
	pinMode (testLed2, OUTPUT);

  	//lastTimeSelectButtonPressed = millis();   

  	// Valores iniciales de la configuracion del TL
  	// sequenceDuration= 10; // 10 segundos
  	// timelapseDuration= 30; // 30 minutos
	
	/* Inicializacion de las variables globales*/
	lastEncoded = 0;
	encoderValue = 0;
	lastencoderValue = 0;
	lastMSB = 0;
	lastLSB = 0;
	changeMenuItem(0);



	//lastTimeSelectButtonPressed=0;	// Control botón Select
	selectButtonStatus=HIGH;

	
	STATE=SCN01;                  	// Estado inicial  
	PROGRAM_STATE=RTM00;			// Estado programa inicial
	
	direction = HIGH;				// Dirección

	currentTLInterval = 1;
	currentTLDistance = 0;        


	rotaryEncoderSetup();
	
	readConfigFromEEPROM();
  

}

/***********************************************************************************/
void loop() {
  // picture loop
  	u8gfirstpage();

  	//drawSplashScreen();

  	do {
    	draw();
  	} 
  	while(u8gnextpage());

	/* Procesar todos los eventos */
	processEvents ();
	// #ifdef DEBUG
	// Serial.print ("processEvents: "); Serial.println (millis());
	// #endif
}

/***********************************************************************************/
/* Render wrapper de pantalla */
void draw(void) {
  // Serial.print ("MenuLevel: "); Serial.print (menuLevel);Serial.print("\n");      
	switch (STATE) {	
		case SCN00:
		drawSplashScreen();
		break;

		case SCN01:      
		// Preparamos variables de gestion del viewport
		if (menuItem>=vpIndex+maxElements) // Avanzamos el viewport
			vpIndex++;
		if (menuItem<vpIndex) 
			vpIndex=menuItem;						// Retrocedemos el viwport

		vpOffset = menuItem-vpIndex;

		drawMenu(menuLevel0.str[config.language], menuLevel0.arrayLen);
		renderVersionNumber ();
		
		break;
		
		case SCN02S01:
		drawConfiguration();
		break;

		case SCN02S02:
		drawConfiguration();
		break;

		case SCN11S01:
		drawRailLen();
		break;
		case SCN11S02:
		drawRailLen();
		break;
		case SCN11S03:
		drawRailLen();
		break;
		case SCN11S04:
		drawRailLen();
		break;


		case SCN03S01:
		drawTimeLapse();
		break;             

		case SCN03S02:
		drawTimeLapse();
		break;             
		
	   	case SCN04S01:
		//newDrawTimeLapsePro();
		drawTimeLapsePro();
		break;
	                              
		case SCN04S02:
		//newDrawTimeLapsePro();
		drawTimeLapsePro();
		break;
		                   
		case SCN04S03:
		//newDrawTimeLapsePro();
		drawTimeLapsePro();
		break;
		
		case RVM:
		drawRVM();
		break;    
		
		case RTM: 
		drawRTM();         
		break;

		case SCN05S01:
		drawStopMotion();
		break;

		case SCN05S02:
		drawStopMotion();
		break;

		case SCN06S01:
		drawVideoPro();
		break;

		case SCN06S02:
		drawVideoPro();
		break;
		
		case SCN06S03:
		drawVideoPro();
		break;		
		
		case SCN06S04:
		drawVideoPro();
		break;		

		case SCN06S05:
		drawVideoPro();
		break;		

		case SCN06S06:
		drawVideoPro();
		break;		

		case RVMP:
		drawVideoPro();
		break;		
		
		case SCN07S01:
		drawAstroFocus();
		break;		

		case SCN07S02:
		drawAstroFocus();
		break;		

		case SCN07S03:
		drawAstroFocus();
		break;		

		case SCN07S04:
		drawAstroFocus();
		break;		

		case SCN07S05:
		drawAstroFocus();
		break;		

		case SCN07S06:
		drawAstroFocus();
		break;	

		case SCN07S07:
		drawAstroFocus();
		break;	

		case SCN07S08:
		drawAstroFocus();
		break;	

		case SCN07S09:
		drawAstroFocus();
		break;	

		case RAFM:
		drawAstroFocus();
		break;	

		case SCN08S01:
		drawFollowFocus();
		break;

		case SCN09S01:
		drawSelectPoint();
		break;
		case SCN09S02:
		drawSelectPoint();
		break;
		case SCN09S03:
		drawSelectPoint();
		break;
		case SCN09S04:
		drawSelectPoint();
		break;
		case SCN09S05:
		drawSelectPoint();
		break;
		case SCN09S06:
		drawSelectPoint();
		break;

		case SCN10S01:
		drawFollowFocusRun();
		break;
		case SCN10S02:
		drawFollowFocusRun();
		break;
		case SCN10S03:
		drawFollowFocusRun();
		break;
	}
}


/***********************************************************************************/  
// Bucle principal donde se procesan los eventos
// Render LCD
// Movimiento motor
// Botón Select
// Sensores fin de carrera 
/***********************************************************************************/  
void processEvents (void) {
	
	/* Controlamos si se ha pulsado botón Select*/
   	//if (digitalRead(selectButtonPin)==LOW) selectButtonPressed();           
	processSelectButton();

 	/* Paramos motor si hemos colisionado */
	// 	if (checkCollision()) stopMotor();        

	// Hacemos los calculos de la rotacion del enconder fuera de la función interrupcion ISR
	// Para mantener la funcion ISR corta
	processEncoderRotation();
	
	/* Comienzo del programa TimeLapse */
	if (STATE==RTM) timeLapseProgramIsRunning(); 

	if (STATE==RVM) videoProgramIsRunning();

	if (STATE==SCN04S02 || STATE ==SCN04S03) calculateTimeLapseParameters();

	if (STATE == SCN06S02 || STATE == SCN06S03) pointIsBeingDefinedVideoProMode();
	if (STATE == SCN07S02 || STATE == SCN07S03) pointIsBeingDefinedAstroFocusMode();
	if (STATE == SCN09S02) pointIsBeingDefinedFollowFocusMode();

	if (STATE == SCN06S05 || STATE == SCN07S09) motorIsMovingToStartPosition();

	if (STATE == RVMP) videoProModeIsRunning();
	if (STATE == RAFM) astroFocusModeIsRunning();
	if (STATE == SCN10S02 || STATE == SCN10S03) followFocusModeIsRunning();

	if (STATE == SCN06S06) pingPongModeIsRunning ();
	
	
}  

void processSelectButton (void) {
	// Generamos el evento selectButtonPressed() cuando se suelta el botón
	if (digitalRead(selectButtonPin)==LOW) 
		selectButtonStatus = LOW;
	else {
		if (selectButtonStatus==LOW) 
			selectButtonPressed();
		selectButtonStatus = HIGH;	
	}
}

/***********************************************************************************/
void selectButtonPressed () {
	_debug ("STATE: "); _debug (STATE); _debug (" / ");
	_debug ("menuItem: "); _debugln (menuItem);

	switch (STATE) {	               
		
		// Click select en el Splash Screen
		case SCN00:
		STATE = SCN01;
		break;

		/* Click Select en pantalla principal */
		case SCN01:   
	 	if (menuItem==0)  {// Entramos en modo Running Video Mode
			videoProgramWillRun();
			resetEncoder();
		}

		if (menuItem==1) {
			STATE=SCN06S01;	// Entramos en modo Video Pro
		   	initializeVideoProMode();
		   	resetEncoder();
	   	}

	 	if (menuItem==2) {
	 		STATE=SCN03S01;    // Entramos en modo TimeLapse Seleccion intervalo-empezar
	 		resetEncoder();
	 	}

	 	if (menuItem==3) {
	 		STATE=SCN04S01;  	// Estado TimeLapsePro selección Intervalo-Distancia-Empezar     
	 		resetEncoder();
	 	}

	   	if (menuItem==4) {		// Entramos en modo StopMotion
	   		// Inicializamos el numero del *siguiente* fotograma
	   		currentNumShots=1;
	   		STATE=SCN05S01;
	   		resetEncoder(); 	
	   	}
	   	
	   	if (menuItem==5) {
	   		STATE=SCN07S01;	// Estramos en modo AstroFocus
	   		initializeAstroFocusMode();
	   		resetEncoder();
	   	}

	   	if (menuItem==6) {
	   		STATE=SCN08S01;	// Estramos en modo Follow Focus
	   		initializeFollowFocusMode();
	   		resetEncoder();
	   	}

		if (menuItem==7) {	// Activar freno
			// Ponemos el driver en full-step para poder usar este modo 
			// para medir la intensidad de corriente del motor.
			digitalWrite (A4988ms1Pin, LOW);
			digitalWrite (A4988ms2Pin, LOW);
			digitalWrite (A4988ms3Pin, LOW);
	   		enableDriver();
	   	}

	   	if (menuItem==8) {  // Desactivar freno
	   		disableDriver();
	   	}

	   	if (menuItem==9) {
	   		STATE=SCN02S01;     // Entramos en el menu configuracion
	 		resetEncoder();
	 	}
		break;
		            
		/* Click Select en pantalla cambio de configuración */
		case SCN02S01: 
	 	if (menuItem == 0) {
	 		STATE=SCN02S02; 	//Entramos a cambiar el idioma
	 		changeMenuItem(config.language);
	 		break;
	 	}
		if (menuItem == 1) {		// Cambiamos el sentido de giro 'insitu'
	 		changeEncoderSpin();
	 		//resetEncoder();
	 		break;
	 	}
	 	if (menuItem == 2) {	// Configuracion de longitud rail
	 		// Pasamos a las variables "locales"
	 		selectedRailLenghtHundreds = config.railLenght / 1000;
	 		selectedRailLenghtTenths = (config.railLenght % 1000) /100;
	 		selectedRailLenghtUnits = (config.railLenght % 1000 % 100) / 10;

	 		_debug ("railLenght:"); 
	 		_debug (config.railLenght); _debug (" "); 
	 		_debug (selectedRailLenghtHundreds); _debug (" "); 
	 		_debug (selectedRailLenghtTenths); _debug (" ");
	 		_debugln (selectedRailLenghtUnits); 
	 		STATE = SCN11S01;
	 		resetEncoder();
	 		break;
	 	}

		if (menuItem==menuConfiguration.numElements) {
	 		STATE=SCN01; 	//Volvemos al menu principal
	 		resetEncoder();
	 		break;
	 	}
		
		/* Clic select en modo cambio de idioma del menu configuracion */
		case SCN02S02:	// Cambio de idioma
			changeLanguage (menuItem);
			STATE=SCN02S01;
			break;
		

		/* Click Select en Modo TimeLapse          */
		case SCN03S01:
		if (menuItem==0) {   			// Vamos a la selección del intervalo
			STATE=SCN03S02;
			resetEncoder();   
			changeMenuItem (currentTLInterval); // Recuperamos el valor previo
			break;
		}       	                                                         
			
		// run
		if (menuItem==1) {    			// Ejecutamos el programa RTM
			STATE=RTM;
			timeLapseProgramWillRun();
			break;
		}
		
		if (menuItem==2) {				// Solo posible porque tiene hasBackButton    
			STATE=SCN01;				// Volvemos a la página principal
			resetEncoder();
			break;
		}
		
		
		case SCN03S02:					// Se ha seleccionado un intervalo
		//currentTLInterval = menuItem;	// Guardamos el valor del intervalo seleccionado
		STATE = SCN03S01;				// Volvemos al menu general Timelapse                        
		resetEncoder();   
		changeMenuItem(1);					// Nos posicionamos en el boton de EMPEZAR
		break;                               
		
		/* Click Select en Modo TimeLapse PRO */ 
		case SCN04S01:
		if (menuItem==0) {
			STATE = SCN04S02;   		// Vamos a la selección del Intervalo
			resetEncoder();
			//changeMenuItem(currentTLInterval);	// Si tenemos seleccionado un valor previo lo recuperamos
			break;
		}	
		if (menuItem==1) {
			STATE = SCN04S03;   		// Vamos a la selección de la distancia  
			resetEncoder(); 
			changeMenuItem(currentTLDistance); // Si tenemos seleccionado un valor previo lo recuperamos 
			break;
		}
		if (menuItem==2) {
			STATE = RTM; 		 		// Comenzamos ejecución del programa RTM
			timeLapseProProgramWillRun();
			break;
		}
		
		// Se hace clic en el botón BACK. Volvemos a la pantalla princial		
		if (menuItem==3) {					// Solo posible porque tiene hasBackButton
			STATE = SCN01; 	 				// Volvemos a la página principal
			resetEncoder();  
		}			
		break;
		
		case SCN04S02: 						// Se ha seleccionado un intervalo. 
		//currentTLInterval = menuItem;       // Guardamos el intervalo seleccionado
		STATE=SCN04S01; 					// Vamos al menu general TimeLapse Pro 
		resetEncoder();  
		calculateTimeLapseParameters();
		break;
		
		case SCN04S03:                      // Se ha seleccionado una distancia
		currentTLDistance = menuItem;		// Guardamos la distancia seleccionada
		STATE=SCN04S01;						// Vamos al menu general TimeLapse Pro
		resetEncoder();
		changeMenuItem(1);						// Devolvemos el foco a la opción de Distancia
		calculateTimeLapseParameters();
		break;
		
		/* Click Select en Running modo video */
		case RVM:                               
		videoProgramWillStop();
		STATE=SCN01; 						// Cambiamos estado
		resetEncoder();
		
		break;
		
		/* Click Select en Running modo timelapse */	
		case RTM:   
		timeLapseProgramWillStop();
		STATE=SCN01; 						// Volvemos al menu principal
		resetEncoder();               
		break;         

		/* Click Select en Modo StopMotion          */
		case SCN05S01:
		if (menuItem==0) {   			// Vamos a la selección de la distancia
			STATE=SCN05S02;
			resetEncoder();   
			changeMenuItem (currentSMDistance); // Recuperamos el valor previo
			break;
		}       	                                                         
			
		// shoot
		if (menuItem==1) {    			// Disparamos y movemos
			//STATE=RTM;
			stopMotionProgramWillRun();
			break;
		}
		
		// Salimos del modo StopMotion
		if (menuItem==2) {				// Solo posible porque tiene hasBackButton    
			STATE=SCN01;				// Volvemos a la página principal
			resetEncoder();
			//disableDriver();
			configureMicrosteping(16);
			break;
		}
		        
		case SCN05S02:	// Hemos seleccionado una distancia
			currentSMDistance = menuItem;	// Guardamos el valor seleccionado
			STATE = SCN05S01;	// Volvemos al menu general Timelapse                        
			resetEncoder();   
			changeMenuItem(1);	// Nos posicionamos en el boton de disparo
			break;	

		// Click en modo Video Pro
		case SCN06S01:	
		if (menuItem==menuVideoPro.numElements) {			//Salimos al menu principal
			STATE=SCN01;
			resetEncoder();
			//disableDriver();
			break;
		}
		if (menuItem==0) {			// Entramos a seleccionar el punto de entrada
			STATE = SCN06S02;
			resetEncoder();
			pointDefineWillRun();
			break;
		}
		if (menuItem==1) {			// Entramos a seleccionar el punto de salida
			STATE = SCN06S03;
			resetEncoder();
			pointDefineWillRun();
			break;
		}
		if (menuItem==2) {			// Entramos a seleccionar la duración
			// Primero comprobamos que podemos entrar en este modo
			if (durationIntervalIsSet) {
				STATE = SCN06S04;	
			}
			break;
		}
		if (menuItem==3) {			// Ejecutamos modo Video Pro
			if (inPointIsSet && outPointIsSet && currentPosition!=IN) {
				STATE = SCN06S05;  // Viaje al punto de entrada
				motorWillMoveToStartPosition();
			}
			else
				videoProModeWillRun();
			break;
		}
		if (menuItem==4) {	// Ejecutamos el Modo Ping-Pong
			if (inPointIsSet && outPointIsSet ) {
				pingPongModeWillRun();
			}
		}
		break;
		
		case SCN06S02:		/* Clic select en la seleccion del punto de entrada */
		STATE=SCN06S01; 	// Volvemos al menu principal del modo
		resetEncoder();
		changeMenuItem(0);
		inPointIsDefined ();
		// Definimos el la duración maxima y minima
		if (outPointIsSet) defineDurationInterval();
		calculateVideoProParameters ();
		break;
		
		/* Clic select en la seleccion del punto de salida */
		case SCN06S03:
		STATE=SCN06S01; 	// Volvemos al menu principal del modo
		resetEncoder();
		outPointIsDefined();
		// Definimos el la duración maxima y minima
		if (outPointIsSet) defineDurationInterval();
		changeMenuItem(2);
		calculateVideoProParameters ();
		break;
		
		/* Clic select en la seleccion de la duración */
		case SCN06S04:
		STATE=SCN06S01; 	// Volvemos al menu principal del modo
		calculateVideoProParameters ();
		changeMenuItem (3);
		break;

		/* Clic mientras nos movemos hasta el comienzo*/
		case SCN06S05:
		STATE=SCN06S01; 
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	// Habilitamos encoder
	  	enableEncoder();
	  	changeMenuItem (3);
		//calculateVideoProParameters ();
		break;

		/* Click mientras nos movemos en ping-pong */
		case SCN06S06:
		
		pingPongModeWillStop ();
		// Habilitamos encoder
	  	enableEncoder();
		STATE=SCN06S01;
		changeMenuItem (0);
		break;

		// Click en modo AstroFocus
		case SCN07S01:	
		if (menuItem==9) {			//Salimos al menu principal
			STATE=SCN01;
			resetEncoder();
			//disableDriver();
			break;
		}
		if (menuItem==0) {			// Entramos a seleccionar el punto de entrada
			STATE = SCN07S02;
			resetEncoder();
			pointDefineWillRun();
			break;
		}
		if (menuItem==1) {			// Entramos a seleccionar el punto de salida
			STATE = SCN07S03;
			resetEncoder();
			pointDefineWillRun();
			break;
		}
		
		if (menuItem == 2) { // Cambiamos de Ida a Ida  y vuelta o viciversa
			
			if (astroRoundTrip != 0)
				astroRoundTrip = 0;
			else
				astroRoundTrip = 1;

			calculateAstroFocusParameters(); //Para calcular la duracion real
			changeMenuItem (3);
			break;
		}

		if (menuItem==3) {			// Entramos a seleccionar la duración en HORAS
			STATE= SCN07S08;
			resetEncoder();
			break;
		}
		if (menuItem==4) {			// Entramos a seleccionar la duración en DECENAS MINUTOS
			STATE= SCN07S07;
			resetEncoder();
			break;
		}
		if (menuItem==5) {			// Entramos a seleccionar la duración en UNIDADES MINUTO
			STATE= SCN07S06;
			resetEncoder();
			break;
		}
		if (menuItem==6) {			// Entramos a seleccionar la duración en DECENAS SEGUNDO
			STATE= SCN07S05;
			resetEncoder();
			break;
		}
		if (menuItem==7) {			// Entramos a seleccionar la duración en UNIDADES SEGUNDO
			STATE= SCN07S04;
			resetEncoder();
			break;
		}
		

		if (menuItem==8) {			// Ejecutamos modo AstroFocus
			
			if (inPointIsSet && outPointIsSet && currentPosition!=IN) {
				_debugln ("Ir a inicio");
				STATE = SCN07S09;  // Viaje al punto de entrada
				motorWillMoveToStartPosition();
			}
			else {
				_debugln ("Start");
				astroFocusModeWillRun();
			}
			break;
		}
		// if (menuItem==4) {	// Ejecutamos el Modo Ping-Pong
		// 	if (inPointIsSet && outPointIsSet ) {
		// 		pingPongModeWillRun();
		// 	}
		// }
		//break;
		
		case SCN07S02:		/* Clic select en la seleccion del punto de entrada */
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		resetEncoder();
		changeMenuItem(1);
		inPointIsDefined ();
		// Definimos el la duración maxima y minima
		if (outPointIsSet) defineDurationInterval();
		calculateAstroFocusParameters ();
		break;
		
		/* Clic select en la seleccion del punto de salida */
		case SCN07S03:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		resetEncoder();
		outPointIsDefined();
		// Definimos el la duración maxima y minima
		if (outPointIsSet) defineDurationInterval();
		changeMenuItem(2);
		calculateAstroFocusParameters ();
		break;
		
		/* Clic select en la seleccion de la duración UNIDADES SEGUNDO*/
		case SCN07S04:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		calculateAstroFocusParameters ();
		changeMenuItem (7);
		break;

		/* Clic select en la seleccion de la duración DECENAS SEGUNDO*/
		case SCN07S05:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		calculateAstroFocusParameters ();
		changeMenuItem (6);
		break;

		/* Clic select en la seleccion de la duración UNIDADES MINUTO*/
		case SCN07S06:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		calculateAstroFocusParameters ();
		changeMenuItem (5);
		break;

		/* Clic select en la seleccion de la duración DECENAS MINUTO*/
		case SCN07S07:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		calculateAstroFocusParameters ();
		changeMenuItem (4);
		break;

		/* Clic select en la seleccion de la duración HORAS*/
		case SCN07S08:
		STATE=SCN07S01; 	// Volvemos al menu principal del modo
		calculateAstroFocusParameters ();
		changeMenuItem (3);
		break;

		/* Clic mientras nos movemos hasta el comienzo*/
		case SCN07S09:
		STATE=SCN07S01; 
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	// Habilitamos encoder
	  	enableEncoder();
	  	changeMenuItem (3);
		break;

		// /* Click mientras nos movemos en ping-pong */
		// case SCN07S06:
		// TIMSK1 &= ~(1<<OCIE1A);	
		// // Habilitamos encoder
	 //  	enableEncoder();
		// STATE=SCN07S01;
		// changeMenuItem (4);
		// break;


		/* Click mientras ejecutamos programa Video Pro*/
		case RAFM:
		TIMSK1 &= ~(1<<OCIE1A);	
		// Habilitamos encoder
	  	enableEncoder();
		STATE=SCN07S01;
		changeMenuItem (3);
		break;


		// Click en modo Follow Focus
		case SCN08S01:	
		if (menuItem==numFocusPointsDefined()+2) {			// Salimos al menu principal
			STATE=SCN01;
			resetEncoder();
			//disableDriver();
			break;
		}

		if (menuItem==numFocusPointsDefined()+1 || menuItem==6) {			// Ejecutamos FollowFocus
			
			// No entramos si solo hay un punto definido
			if (numFocusPointsDefined() > 1) {
				STATE=SCN10S01;
				resetEncoder();
			}
			break;
		}

		if (menuItem >=0 && menuItem<=numFocusPointsDefined()) {		// Entramos a definir la posicion del punto
			pointDefinitionWillCall();
			
			STATE=SCN09S01;
			resetEncoder();
			break;
		}

		// Click en modo Definicion de punto
		case SCN09S01:
		if (menuItem==0) {	// Entrando en la definicion de la posicion el punto.
			resetEncoder();
			STATE=SCN09S02;
			break;
		}
		if (menuItem==1) {	// Entramos a definir decenas de segundo
			resetEncoder();
			STATE=SCN09S04;
			break;
		}
		if (menuItem==2) {	// Entramos a definir segundos
			resetEncoder();
			STATE=SCN09S05;
			break;
		}
		if (menuItem==3) {	// Entramos a definir decimas de segundo
			resetEncoder();
			STATE=SCN09S06;
			break;
		}

		if (menuItem==4) { // Pulsamos OK, hemos terminado de definir un punto.
			if (pointIsCompletelyDefined() ) {
				resetEncoder();	
				STATE=SCN08S01;
				// Debug focus points
				for (unsigned char i=0;i<6;i++) {
					_debug (focusPoints[i].isDefined);
					_debug (":");
					_debug (focusPoints[i].steps);
					_debug (":");
					_debug(focusPoints[i].durationTens);
					_debug(focusPoints[i].durationUnits);
					_debug (",");
					_debugln(focusPoints[i].durationTens);
				}
			}

			// Dejamos el cursor en el siguiente punto sin definir
			changeMenuItem (numFocusPointsDefined());
			break;
		}

		case SCN09S02: 		
		if (menuItem==0) 	{// Hemos terminado de definir la posicion del punto
			if (pointPositionIsDefined())
				STATE=SCN09S01;
		}
		else {			// Click durante el movimiento del motor. Paramos de movernos, vamos a punto muerto
			changeMenuItem (0);
		}
		break;

		case SCN09S04:		// Fin definicion duracion Decenas 
		STATE=SCN09S01;
		changeMenuItem(2);
		break;

		case SCN09S05:		// Fin definicion duracion Unidades 
		STATE=SCN09S01;
		changeMenuItem(3);
		break;

		case SCN09S06:		// Fin definicion duracion Centesimas
		STATE=SCN09S01;
		changeMenuItem(4);
		break;


		// Pantalla ejecución modo Follow Focus
		case SCN10S01:
		if (menuItem==2) {	// Volvemos al menu anterior
			STATE=SCN08S01;
			resetEncoder();
			break;
		}

		if (menuItem==0) {	// Ir a inicio
			if (modeFollowFocusWillMoveToStart())
				STATE = SCN10S02;
			else
				changeMenuItem (1);
			break;
		}

		if (menuItem==1) {	// Mover a punto a punto
			if (canMovePointToPoint()) {
				modeFollowFocusWillRun();
				STATE = SCN10S03;
			}
			else
				changeMenuItem(0);
			break;
		}

		case SCN10S02: 	// Click mientras nos moviamos hasta el principio.
		// Ajustamos la posicion a Desconocida
		inLimbo=true;

		STATE=SCN10S01; 
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	// Habilitamos encoder
	  	enableEncoder();
	  	changeMenuItem (0);
		break;

		case SCN10S03:	// Click mientras nos moviamos dentro de un tramo
		// Ajustamos la posicion a Desconocida
		inLimbo=true;

		STATE=SCN10S01; 
		//// Deshabilitamos la interrupción (paramos motor)
	  	TIMSK1 &= ~(1<<OCIE1A);	
	  	// Habilitamos encoder
	  	enableEncoder();
	  	changeMenuItem (0);
		break;

		case SCN11S01: // Pantalla de configuracion de longitud rail
			if (menuItem == 4) { // Back
				STATE = SCN02S01; //Devolvemos el controla a la pantalla anterior
				break;
			}
			if (menuItem == 0) {
				STATE = SCN11S02; // Centenares de cm
				break;
			}
			if (menuItem == 1) {
				STATE = SCN11S03; // Decenas de cm
				break;
			}
			if (menuItem == 2) {
				STATE = SCN11S04; // Unidades de cm
				break;
			}
			if (menuItem == 3) { // Validar entrada de longitud
				 // Si hay error en la validacion no salimos del modo.
				if (validateAndSaveRailLenght2EEPROM())
					STATE = SCN02S01; 
				break;
			}
		break;

		case SCN11S02: // Centenares de cm
		STATE = SCN11S01;
		changeMenuItem (1);
		break;
		
		case SCN11S03: // decenas de cm
		STATE = SCN11S01;
		changeMenuItem (2);
		break;

		case SCN11S04: // Unidades de cm
		STATE = SCN11S01;
		changeMenuItem (3);
		break;


	}                                                      
}	


void timeLapseProgramWillRun (void) {
	PROGRAM_STATE=RTM01;                                                                
	// Preparamos los datos del programa a ejecutar
	programParameters.speed =  defaultSpeed;
	programParameters.intervalDuration = currentTLInterval * 1000;    
	//programParameters.motorMoveDuration = menuTimeLapse.interval[currentTLInterval] * pctMotorMoveDuration;
	
	// Hacemos fija la duración del movimiento del motor
	programParameters.motorMoveDuration = TLmotorMoveDuration;


	// Timer duración entre disparos
	timerShootInterval=millis();
	// Timer de las animaciones
	timerAnimation=millis();     // timer usado en las animaciones
	// Timer duración del trigger de la camara
	//timerTriggerPulseWidth =millis();

	enableDriver();
	
	resetEncoder();  

	// Inicializamos el timer usado en le primer estado
	//timerMotorMove=millis();
	timerAnimation=millis();     // timer usado en las animaciones
	timerShootInterval=millis();
	timerMotorMove=timerShootInterval;

	// Configuramos micropasos
	configureMicrosteping(16);

	// Inicializamos el contador de disparos
	currentNumShots=0;
	
	//currentStepperSpeed = convertPWM2TimerRegister(programParameters.speed);
	currentStepperSpeed=TLSpeed;
	
	// Inicializamos el contador de pointIsBeingDefinedAstroFocusMode
	stepCounter= TLDistanceSteps; 
	steps=0;

	// Arrancamos el motor
	enableTimer1Interrupt();

	digitalWrite (trigger1,LOW);
	digitalWrite (trigger2,LOW);

	_debug ("Entrando RTM01: "); _debugln (millis());
}

void timeLapseProProgramWillRun (void) {
	PROGRAM_STATE=RTM01;      
	// Preparamos los datos del programa a ejecutar
	programParameters.speed = menuTimeLapsePro.distance[currentTLDistance];
	programParameters.intervalDuration = currentTLInterval * 1000; // En milisegundos
	//programParameters.motorMoveDuration = menuTimeLapsePro.interval[currentTLInterval] * pctMotorMoveDuration;
	programParameters.motorMoveDuration = TLmotorMoveDuration;

	enableDriver();
	
	resetEncoder();  

	// Inicializamos el timer usado en le primer estado
	//timerMotorMove=millis();
	timerAnimation=millis();     // timer usado en las animaciones
	timerShootInterval=millis();
	timerMotorMove=timerShootInterval;

	// Inicializamos el contador de disparos
	currentNumShots=0;
	
	// Configuramos micropasos
	configureMicrosteping(16);
	
	// Inicializamos el contador de pasos
	stepCounter = menuTimeLapsePro.distance[currentTLDistance];
	steps=0;
	
	currentStepperSpeed=TLSpeed;	

	//Arrancamos el motor
	enableTimer1Interrupt();

}

void timeLapseProgramIsRunning() {
	// Movimiento - Trigger - Pausa
	// Bucle principal, intervalo entre disparos
	if (millis() - timerShootInterval < programParameters.intervalDuration) {	
		switch (PROGRAM_STATE) {

			case RTM01: // Movimiento del motor
			processDirection();
			if (steps>=stepCounter) {
				steps=0;
				PROGRAM_STATE= RTM03;
				// Inicializamos timer siguiente estado
				timerTriggerPulseWidth =millis();
				disableTimer1Interrupt();
			}
			break;

			case RTM03:	// Trigger camara
			
			// Generamos pulso para el disparador de la camara
			digitalWrite (trigger1,HIGH);
			digitalWrite (trigger2,HIGH);
			digitalWrite (testLed2,HIGH);		
			if (millis() - timerTriggerPulseWidth >= triggerPulseWidth) {
	   			digitalWrite (trigger1,LOW);
	   			digitalWrite (trigger2, LOW);
	   			digitalWrite (testLed2, LOW);	

	   			// Incrementamos el numero de disparos realizados
	   			currentNumShots++;
				// Siguiente estado: espera hasta que termine el intervalo
				PROGRAM_STATE=RTM04;

			}
			break;
		}
	} else {
		// Hemos llegado al final del intervalo actual

		//Sigueinte estado
		PROGRAM_STATE=RTM01;
		
		// Inicializamos el timer para el siguiente intervalo
		timerShootInterval=millis();
		// inicializamos timer del movimiento del motor
		timerMotorMove=timerShootInterval;
		
		//currentStepperSpeed = convertPWM2TimerRegister(programParameters.speed);
		
		//Arrancamos el motor
		enableTimer1Interrupt();
		//_debug ("Entrando RTM01: "); _debugln (millis());

	}
}          

void timeLapseProgramWillStop(void){
	disableTimer1Interrupt();
	//disableDriver();
	PROGRAM_STATE=RTM00; 
	digitalWrite (trigger1,LOW);  
	digitalWrite (trigger2,LOW);
}

void stopMotionProgramWillRun(void) {  // OJO Funcion bloqueante	
	//
	enableDriver();

	// Configuramos micropasos
	configureMicrosteping (2);

	// Chequeamos mando de dirección
	processDirection();
	
	// Disparamos
	digitalWrite (trigger1,HIGH);
	digitalWrite (trigger2,HIGH);
	digitalWrite (testLed2,HIGH);		
	delay (triggerPulseWidth);
	digitalWrite (trigger1,LOW);
	digitalWrite (trigger2,LOW);
	digitalWrite (testLed2, LOW);	

	// Incrementamos contador de disparos
	currentNumShots++;

	// Delay
	delay(250);

	// Movemos
	moveMotorSteps (menuStopMotion.distance[currentSMDistance]);

}



int checkCollision (void) {
	//if ((direction==LEFT && checkCollision()==collisionLEFT) ||  (direction==RIGHT && checkCollision()==collisionRIGHT)) 
	return collisionNONE; // DEBUG
}   


void calculateTimeLapseParameters(void) {

	// Convertimos numeros de pasos a distancia lineal en mm en cada intervalo
  	float distancePerShoot = menuTimeLapsePro.distance[currentTLDistance]*0.003125;

  	// Numero de disparos durante el recorrido de todo el rail.
  	//float numShoots = config.railLenght / distancePerShoot;
	float numShoots = config.railLenght / distancePerShoot;
  	// Tiempo que se tarda en recorrer todo el rail
  	unsigned long sequenceDuration = currentTLInterval * numShoots;

	snprintf(sSequenceDuration, sizeof(sSequenceDuration), "%u:%u:%u \n", (unsigned int) (unsigned long)(sequenceDuration / 3600), (unsigned int)(unsigned long) (sequenceDuration % (3600)) / 60, (unsigned int)(unsigned long)((sequenceDuration % 3600) % 60));

	_debugln ("sSequenceDuration");
	_debug ("numShoots: "); _debugln (numShoots);
	_debug ("distancePerShoot: "); _debugln (distancePerShoot);
	_debug ("currentTLInterval: "); _debugln (currentTLInterval);
	_debug ("rail: "); _debugln (config.railLenght);
	_debug ("sequenceDuration: "); _debugln (sequenceDuration);
	//_debugln (sequenceDuration);

  // Serial.print ("DistancePerShoot: "); Serial.println (distancePerShoot);
  // Serial.print ("numShoots: "); Serial.println (numShoots);
  // Serial.print ("Duration: "); Serial.println (duracion);
  // snprintf(buffer, sizeof(buffer), "%d:%d:%d \n", (int) duracion / (60 * 60), (int)   (duracion % (60*60)) / 60, (int) duracion % (60*60) % 60);
  // Serial.print ("Duration: "); Serial.println (buffer);  

}

/***********************************************************************************/    
/* Hacemos ciclica la selección de los menus con el encoder*/
void makeSelectionCiclyc(void) {
  int max;
  int min;
  switch (STATE) {  
    case SCN01:     /* Menu principal */ 
    // No hacemos ciclica
    max= menuLevel0.arrayLen-1; 
    if (menuItem > max) changeMenuItem(max);

    if (menuItem < 0 ) changeMenuItem(0);
    break;    
    
    case SCN02S01:     /* Menu configuracion */ 
    max=menuConfiguration.numElements;   // Habilitamos el uso del boton Back
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;                                                              
    
    case SCN02S02:     /* Cambio de idioma en Menu configuracion */ 
    max=LANGUAGES-1;   
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;                                                              
    

    case SCN03S01:     /* Menu seleccion tiempo entre disparos modo Timelapse */
    if (menuTimeLapse.hasBackButton)
      max=menuTimeLapse.strLen;     // Habilitamos uso del boton Back 
    else
      max=menuTimeLapse.strLen-1;
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;       
    
    case SCN03S02:    // Seleccion de los segundos de intervalo en Modo Timelapse
    max=menuTimeLapse.strIntervalLen-1;
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;
              
    case SCN04S01:
    if (menuTimeLapsePro.hasBackButton)
      max=menuTimeLapsePro.strLen; // Habilitamos el uso del boton Back
    else
      max=menuTimeLapsePro.strLen-1;
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;                                                   
               
    case SCN04S02: //Seleccion de la duración de la secuencia
    max=menuTimeLapsePro.strIntervalLen -1;
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
    break;
    
    case SCN04S03: 
    max=menuTimeLapsePro.strDistanceLen -1;
    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
    break;
    
    case RVM:       /* Pantalla Running Video Mode */  
    max=menuVideo.arrayLen-1; 
    if (menuItem > max) changeMenuItem(max);
    if (menuItem < 0)  changeMenuItem(0);
    break;                                                                      
    
    case RTM:       /* Pantalla Running TimeLapse Mode */   
    break;

    case SCN05S01:	// Menu principal modo Stop Motion
    if (menuStopMotion.hasBackButton)
		max=menuStopMotion.strLen;
	else
		max=menuStopMotion.strLen-1;

    if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
    break;

    case SCN05S02: // Seleccion de la distancia
    	max=menuStopMotion.strDistanceLen -1;
    	if (menuItem > max)  changeMenuItem(0);
    	if (menuItem < 0) changeMenuItem(max);
    break;

    case SCN06S01: // Menu principal modo Video Pro
    if (menuVideoPro.hasBackButton)
    	max=menuVideoPro.numElements;
    else
    	max=menuVideoPro.numElements-1;
	if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
	break;	
    
    case SCN06S02: // Seleccion punto entrada
    max =  3;
    min = -3;
    if (menuItem<min) changeMenuItem (min);
    if (menuItem>max) changeMenuItem (max);
    break;

    case SCN06S03: // Seleccion punto salida
    max =  3;
    min = -3;
    if (menuItem<min) changeMenuItem (min);
    if (menuItem>max) changeMenuItem (max);
	break;

    case SCN06S04: // Seleccion duración
    // max =  3;
    // min = -3;
    // if (menuItem<min) changeMenuItem (min);
    // if (menuItem>max) changeMenuItem (max);
    break;
  

	case SCN07S01: // Menu principal modo AstroFocus
    max=9;

	if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
	break;	
    
    case SCN07S02: // Seleccion punto entrada
    max =  3;
    min = -3;
    if (menuItem<min) changeMenuItem (min);
    if (menuItem>max) changeMenuItem (max);
    break;

    case SCN07S03: // Seleccion punto salida
    max =  3;
    min = -3;
    if (menuItem<min) changeMenuItem (min);
    if (menuItem>max) changeMenuItem (max);
	break;

    case SCN07S04: // Seleccion duración
    // max =  3;
    // min = -3;
    // if (menuItem<min) changeMenuItem (min);
    // if (menuItem>max) changeMenuItem (max);
    break;

    case SCN08S01:
    if (numFocusPointsDefined()==6)
    	max = 7;
    else
    	max= numFocusPointsDefined()+2;

	if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
	break;	

	case SCN09S01:	// Definicion de punto. Pantalla principal
	max=4;
	if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max)  ;
	break;		

	case SCN09S02:	// Definición de la posicion del punto
	min=-3;
	max=3;
	if (menuItem>max) changeMenuItem(max);
	if (menuItem<min) changeMenuItem(min);
	break;

	case SCN10S01:	// Definición de la posicion del punto
	min=0;
	max=2;
	if (menuItem>max) changeMenuItem(min);
	if (menuItem<min) changeMenuItem(max);
	break;

	case SCN11S01: // Menu configuracion longitud rail
    max=4;
	if (menuItem > max)  changeMenuItem(0);
    if (menuItem < 0) changeMenuItem(max);
	break;	
  } 
}           

void changeMenuItem (int v) {
	menuItem=v;
	RTmenuItem=v;
}

void changeEncoderSpin () {
	/* Invertimos el valor  */
	config.encoderSpin = !config.encoderSpin;
	EEPROM.write (eeEncoderSpinAddr, config.encoderSpin); 
}

void changeLanguage (int value) {
	if (config.language != value)
		if (value >= 0 && value < LANGUAGES) {
			config.language=value;
			EEPROM.write (eeLanguageAddr, value);
		}
}

void readConfigFromEEPROM (void){
	/* Leemos de la EEPROM los valores de la configuración */
	int l = EEPROM.read (eeLanguageAddr);
	// Sanity check
	if (l >= 0 && l < LANGUAGES)
		config.language=l;
	else
		config.language=0; // Dejamos idioma por defecto
	

	l = EEPROM.read (eeEncoderSpinAddr);
	if (l == 0 || l == 1) 
		config.encoderSpin=l;


	// Longitu del rail.
	// En la eeprom guardamos 3 bytes con las centenas, decenas y unidades de cm
	// En la variable config.railLenght guardamos en mm.

	
	byte c = EEPROM.read (eeRailLenghtC);
	byte d = EEPROM.read (eeRailLenghtD);
	byte u = EEPROM.read (eeRailLenghtU);
	
	// Validar que c, d y u estan entre 0 y 9
	char err =0;
	if (c<0 || c > 9) err =1;
	if (d<0 || d > 9) err =1;
	if (u<0 || u > 9) err =1;
		
	// Si hay alguna incongruencia en la lectura, grabamos un valor por defecto para la longitud
	if (err)
		config.railLenght =850; // Valor por defecto
	else
		config.railLenght = c*1000 + d*100 + u*10;	// Recuerda que convertimos a mm.

	// Además comprobamos la medida minima
	if (config.railLenght < minRailLenght) 
		config.railLenght = 850; // Por defecto rail de 1 metro.

	_debug ("eeprom railLenght ");
	_debug (c); _debug (" ");
	_debug (d); _debug (" ");
	_debugln (u);


	// ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

char validateAndSaveRailLenght2EEPROM(void) {

	unsigned int l = selectedRailLenghtHundreds *100 + selectedRailLenghtTenths*10 +selectedRailLenghtUnits;

	// Maximo 2 metros y minimo minRailLenght
	if ( l < minRailLenght || l > maxRailLenght)
		return 0; // Error

	// Grabamos eeprom
	EEPROM.write (eeRailLenghtC, selectedRailLenghtHundreds);
	EEPROM.write (eeRailLenghtD, selectedRailLenghtTenths);
	EEPROM.write (eeRailLenghtU, selectedRailLenghtUnits);

	// Actualizamos variable configuracion
	config.railLenght = selectedRailLenghtHundreds*1000 + selectedRailLenghtTenths*100 + selectedRailLenghtUnits*10;
	return 1;

}

