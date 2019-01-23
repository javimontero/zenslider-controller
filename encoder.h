/*
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

//#include "U8glib.h"
#include "global.h"  
//#include "lcdstrings.h" 
#include "dollyController.h"
*/
void rotaryEncoderSetup() {
	pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  	//call updateEncoder() when any high/low changed seen
  	//on interrupt 0 (atmega328:pin 2 / atmega644p: pin 10-->PD2), 
	//or interrupt 1 (atmega328:pin 3 / atmega644p: pin 11-->PD3) 
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

/***********************************************************************************/
void updateEncoder() {
  /*
  Debounce.
  Intentamos controlar el tiempo que ha pasado desde el ultimo disparo del encoder
  En caso de que sea menor que un umbral, lo identificamos como ruido y no hacemos
  nada.
  */
  if (millis()-lastDetent < 2) return;
  
  int MSB = digitalRead(encoderPin1); 	//MSB = most significant bit
  int LSB = digitalRead(encoderPin2); 	//LSB = least significant bit

  int encoded = (MSB << 1) |LSB; 		//converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  /* 
  El valor de RTmenuItem ha podido cambiar durante el procesamiento de alguna función,
  por lo que antes de actualizarlo, lo inicializamos al valor real menuItem
  */
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderValue ++;
    if (encoderValue %4 == 0 ) {
      //RTmenuItem=menuItem;
		  if (config.encoderSpin) RTmenuItem++; else RTmenuItem--;
    }
  }
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderValue --;
    if (encoderValue % 4 == 0 ) {
      //RTmenuItem=menuItem;
		  if (!config.encoderSpin) RTmenuItem++; else RTmenuItem--;
    }
  }
  lastEncoded = encoded; 				//store this value for next time  	
  lastDetent=millis();
  
}


void resetEncoder (void) {
  encoderValue=0;
  RTmenuItem=0;
  menuItem=0;
}            

void disableEncoder (void) {
  detachInterrupt(0);
  detachInterrupt(1);  
}                       

void enableEncoder (void) {
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
}

void processEncoderRotation(void) {
  // Desacoplamos la variable que se utiliza en ISR

  lastMenuItem=menuItem;
  menuItem=RTmenuItem;

  if (lastMenuItem<menuItem) {
    processEncoderPlusClick();
      //lastMenuItem=menuItem;
  }
  if (lastMenuItem>menuItem) {
    processEncoderMinusClick();
      //lastMenuItem=menuItem;
  }
}

void processEncoderPlusClick(){
  makeSelectionCiclyc(); 
  switch (STATE) {
   
    case SCN03S02: //Seleccion intervalo modo TimeLapse
      if (currentTLInterval == TLMaxDuration)  // Maximo
        currentTLInterval = 1;
      else
        currentTLInterval++;
    break;

    case SCN04S02:  // Seleccion intervalo modo TimeLapsePro
    
      if (currentTLInterval == TLMaxDuration)  // Maximo
        currentTLInterval =1;
      else
        currentTLInterval++;
      break;

    case SCN04S03:
    //timelapseDuration++;
    //calculateTimeLapseParameters();
    break;

    case SCN06S04:  // Modo seleccion de la duracion. Video Pro
    selectedDuration++;
      if (selectedDuration > maxDuration)
        selectedDuration=maxDuration; // Definimos la duración minima por defecto
    break;

    // Valores máximos de la duracion en modo ASTROFOCUS
    case SCN07S04:
    selectedDurationSecondsUnits++;
    if (selectedDurationSecondsUnits >9) selectedDurationSecondsUnits=0;
    break;
    
    case SCN07S05:
    selectedDurationSecondsTens++;
    if (selectedDurationSecondsTens >5) selectedDurationSecondsTens=0;
    break;

    case SCN07S06:
    selectedDurationMinutesUnits++;
    if (selectedDurationMinutesUnits >9) selectedDurationMinutesUnits=0;
    break;

    case SCN07S07:
    selectedDurationMinutesTens++;
    if (selectedDurationMinutesTens >5) selectedDurationMinutesTens=0;
    break;
    
    case SCN07S08:
    selectedDurationHours++;
    if (selectedDurationHours > maxDurationHoursAFMode) selectedDurationHours=0;
    break;
    
    // Modo Follow Focus, seleccion de la duración
    case SCN09S04:  // decenas de segundo
    selectedDurationSecondsTens++;
    if (selectedDurationSecondsTens>5) selectedDurationSecondsTens=0;
    break;
    case SCN09S05:  // unidades de segundo
    selectedDurationSecondsUnits++;
    if (selectedDurationSecondsUnits>5) selectedDurationSecondsUnits=0;
    break;
    case SCN09S06:  // centesimas de segundo
    selectedDurationSecondsTenths++;
    if (selectedDurationSecondsTenths>5) selectedDurationSecondsTenths=0;
    break;

    // Configuracion longitud rail
    case SCN11S02:
    selectedRailLenghtHundreds ++;
    if (selectedRailLenghtHundreds > 9) selectedRailLenghtHundreds = 9;
    break;

    case SCN11S03:
    selectedRailLenghtTenths ++;
    if (selectedRailLenghtTenths > 9) selectedRailLenghtTenths = 9;
    break;

    case SCN11S04:
    selectedRailLenghtUnits ++;
    if (selectedRailLenghtUnits > 9) selectedRailLenghtUnits = 9;
    break;

  }
  //calculateTimeLapseParameters();
}

void processEncoderMinusClick() {
  makeSelectionCiclyc();
  switch (STATE) {
  
    case SCN03S02: //Seleccion intervalo modo TimeLapse
      if (currentTLInterval == 1)  // Maximo
        currentTLInterval = TLMaxDuration;
      else
        currentTLInterval--;
    break;


    case SCN04S02:
      if (currentTLInterval == 1) 
        currentTLInterval = TLMaxDuration; //maximo
      else
      currentTLInterval--;

    break;

    case SCN04S03:
    //timelapseDuration--;
    //calculateTimeLapseParameters();
    break;

    case SCN06S04:  // Modo seleccion de la duracion. Video Pro
    selectedDuration--;
    if (selectedDuration < minDuration)
      selectedDuration=minDuration; // Definimos la duración minima por defecto
    break;
 
 
  // Valores minimos de la duracion en modo ASTROFOCUS
    case SCN07S04:
    
    if (selectedDurationSecondsUnits !=0) selectedDurationSecondsUnits--;
    break;
    
    case SCN07S05:
    if (selectedDurationSecondsTens !=0) selectedDurationSecondsTens--;
    break;

    case SCN07S06:
    if (selectedDurationMinutesUnits !=0) selectedDurationMinutesUnits--;
    break;

    case SCN07S07:
    if (selectedDurationMinutesTens !=0) selectedDurationMinutesTens--;
    break;
    
    case SCN07S08:
    if (selectedDurationHours !=0) selectedDurationHours--;
    break;

    // Modo Follow Focus, seleccion de la duración
    case SCN09S04:  // decenas de segundo
    if (selectedDurationSecondsTens !=0) selectedDurationSecondsTens--;
    break;

    case SCN09S05:  // unidades de segundo
    if (selectedDurationSecondsUnits !=0) selectedDurationSecondsUnits--;
    break;

    case SCN09S06:  // centesimas de segundo
    if (selectedDurationSecondsTenths !=0) selectedDurationSecondsTenths--;
    break;

    // Configuracion longitud rail
    case SCN11S02:
    
    if (selectedRailLenghtHundreds != 0) selectedRailLenghtHundreds --;
      
    break;

    case SCN11S03:
    
    if (selectedRailLenghtTenths != 0) selectedRailLenghtTenths --;
    
    break;

    case SCN11S04:
    
    if (selectedRailLenghtUnits != 0) selectedRailLenghtUnits --;
      
    break;

  } 
}


