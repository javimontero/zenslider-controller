#define version "B2.0"

#define DEBUG

#ifdef DEBUG
#define _debug(X) Serial.print(X)
#define _debugln(X) Serial.println(X)
#else
#define _debug(X) //
#define _debugln(X) //


#endif

#ifdef _MAINFILE
#define _extern 
#endif      
#ifndef _MAINFILE
#define _extern extern 
#endif

#define LANGUAGES 2
                    

// Tipos de letra
/***********************************************************************************/
//#define fontH1 u8g_font_fur30r 
#define fontH1 u8g_font_fur11r 
//#define fontH1 u8g_font_fur14r 
#define fontH2 u8g_font_6x13
//#define fontH2 u8g_font_unifont
//#define fontH3 u8g_font_04b_03r
//#define fontH3 u8g_font_micro 
#define fontH3 u8g_font_5x7
//#define fontH4 u8g_font_04b_03
//#define fontH4 u8g_font_5x7



// Estados (STATE)
#define SCN00 	0	/* Splash screen */

#define SCN01 	1	/* Menu principal */

#define SCN02S01 	20   /* Menu Configuracion */
#define SCN02S02	21 	 /* Cambio de idioma */



#define RVM 	5   /* Pantalla Running Video Mode */
#define RTM 	6   /* Pantalla Running TimeLapse Mode */        
#define RVMP	7   /* Programa Running Video Pro*/                    
#define RAFM	8   /* Programa Running Astro Focus Mode */
//#define RFFM	9	/* Programa Running Follow Focus Mode */

#define SCN03S01	50		// Estado TimeLapse selección Intervalo-Empezar-Back
#define SCN03S02	51		// Estado Timelapse selección Intervalo

#define SCN04S01	101	 	// Estado TimeLapsePro selección Intervalo-Distancia-Empezar-Back
#define SCN04S02    102     // Estado TimeLapsePro selección intervalo
#define SCN04S03    103     // Estado TimeLapsePro selección distancia     

#define SCN05S01	201		// Estado StopMotion pantalla principal
#define SCN05S02	202		// Estado StopMotion selección distancia

#define SCN06S01	301		// Estado Video Pro. Pantalla principal
#define SCN06S02	302		// Estado Video Pro. Seleccion punto de entrada
#define SCN06S03	304		// Estado Video Pro. Seleccion punto de salida
#define SCN06S04	305		// Estado Video Pro. Seleccion punto de duracion
#define SCN06S05 	306		// Estado Video Pro. Movimiento hasta punto de inicio
#define SCN06S06 	307		// Estado Video Pro. Movimiento Ping-Pong

#define SCN07S01	401		// Estado Astro Focus. Pantalla principal
#define SCN07S02	402		// Estado Astro Focus. Seleccion punto de entrada
#define SCN07S03	403		// Estado Astro Focus. Seleccion punto de salida
#define SCN07S04	404		// Estado Astro Focus. Seleccion duración unidades segundo
#define SCN07S05	405		// Estado Astro Focus. Seleccion duracion decenas segundo
#define SCN07S06	406		// Estado Astro Focus. Seleccion duracion unidades minuto
#define SCN07S07	407		// Estado Astro Focus. Seleccion duración decenas minuto
#define SCN07S08	408		// Estado Astro Focus. Seleccion duracion horas
#define SCN07S09 	409		// Estado Astro Focus. Movimiento hasta punto de inicio

#define SCN08S01	501		// Estado FollowFocus. Pantalla principal

#define SCN09S01	601		// Definición de punto. Pantalla principal
#define SCN09S02	602		// Seleccion Posición de punto. 
#define SCN09S03	603		// Seleccion de la duración
#define SCN09S04	604 	// Seleccion decenas de segundos de duración
#define SCN09S05	605		// Seleccion segundos de duración
#define SCN09S06	606		// Seleccion decimas de segundo de duración

#define SCN10S01	701		// Ejecución modo FollowFocus. Pantalla principal
#define SCN10S02	702		// Moviendonos hasta el inicio
#define SCN10S03	703		// Moviendose de punto a punto

#define SCN11S01	801		// Pantalla configuracion longitud del rail
#define SCN11S02	802		// Pantalla configuracion longitud rail . Centeneraes de centimetro
#define SCN11S03	803		// Pantalla configuracion longitud rail. Decenas de centimetro
#define SCN11S04	804		// Pantalla configuracion longitud rail. Unidades de centimetro


//#define SCN12S01	901		// Quitar freno

                 
// Estados de programa (PROGRAM_STATE)                           
#define RTM00       200		// Program stopped
#define RTM01		201		// Motor moviendose
#define RTM02       202     // Delay
#define RTM03       203     // Trigger camara
#define RTM04       204     // Delay    

#define RVM00		300		// Program stopped
#define RVM01		301		// Motor running


#define delayRTM02  100		// ms de delay en PROGRAM_STATE = RTM02
#define delayRTM04  100		// ms de delay en PROGRAM_STATE = RTM04                 
#define triggerPulseWidth 200		// ms de delay entre HIGH y LOW (o tiempo en estado HIGH) para que la camara se entere 
                      
#define defaultSpeed 20			// Velocidad por defecto (pwm) para modo TimeLapse normal
#define pctMotorMoveDuration 0.6 	// Porcentaje de movimiento del motor con respecto al intervalo de disparos
#define rampDuration 	400		// Duración de la rampa de aceleracion y deceleración en ms


// Colisiones
#define collisionNONE	0
#define collisionLEFT	1
#define collisionRIGHT	2

// Direccion
#define LEFT 			HIGH
#define RIGHT			LOW

// Animaciones
#define F_ANIMATION 200		// Frecuencia animacion
#define N_ITEMS 10			// Numero de circulos a animar






/***********************************************************************************/

// Velocidades maximas y minimas modo video
//#define minStepperFreqOCR1A 389		// V minima
//#define maxStepperFreqOCR1A 20		// V max

//#define minStepperFreqOCR1A 389	// V minima
//#define maxStepperFreqOCR1A 23		// V max
//#define maxStepperFreqOCR1A 4		// V max

//#define minVideoStepperFreqOCR1A 100	// V minima
//#define maxStepperFreqOCR1A 23		// V max
//#define maxVideoStepperFreqOCR1A 4		// V max

// Distancia estandar en modo TimeLapse
#define TLDistanceSteps 160

// Velocidad estandar en modo TimeLapse y TimeLapsePro
#define TLSpeed 77

// Duracion del movimiento del motor (ms) en modo TimeLapse
#define TLmotorMoveDuration 100 

//_extern float delta; // Pendiente de la curva de aceleración

/***********************************************************************************/
/* Pines Digitales */

#define encoderPin1  10
#define encoderPin2  11

#define selectButtonPin 18

//#define encoderSelectPin 26
#define encoderSelectPin A5

#define lcdMOSI 19
#define lcdSCL  20
#define lcdA0   21
#define lcdRST  22
#define lcdCS   23          

// Inicializacion U8glib para OLED con driver SSD1306
U8GLIB_SSD1306_128X64 u8g(lcdSCL,lcdMOSI,lcdCS,lcdA0,lcdRST); //SCK,MOSI,CS,A0,RST    

#define trigger1 16
#define trigger2 17

#define dirSwitchPin 15

/* Pololu A4988 Stepper driver */  
#define A4988dirPin    24	
#define A4988ms1Pin    14
#define A4988ms2Pin    12
#define A4988ms3Pin    13
#define A4988enablePin 25  
#define A4988stepPin	2       

/* Pines Analogicos */
#define hallRight 1
#define hallLeft  2

/* Test leds*/
#define testLed1 27
#define testLed2 28
// #define ulong unsigned long



/***********************************************************************************/
/* Variables globales*/ 


long lastDetent=millis();	// Rotary Encoder Debounce


volatile char flag_ISR_acel = 0 ;  // Hacemos que se ejecute el codigo de la aceleracion o el estandar dentro de la ISR

volatile long stepCounter=0;
volatile long steps=0; 		// Contador de pasos
volatile long stepsPlus=0;	// Contador de pasos en una dirección
volatile long stepsMinus=0; // Contador de pasos en otra direciión


// TEST ACELERACION

typedef struct {
	unsigned long ac;	// Numero de pasos acelerando
	unsigned long dc;	// Paso en el que comienza la deceleracion
	unsigned int s;		// Velocidad instantanea
} aRampData;

aRampData aRamp;

/* Variables modo Video Pro*/
bool inPointIsSet=false;
bool outPointIsSet=false;
long inPoint;
long outPoint;
long stepsToMove;
int speedToMove;			// Velocidad en OCR
//unsigned int speedToMoveHz;	// Velocidad en Hz
float speedToMoveHz;	// Velocidad en Hz

bool lastPointSet;
int tacel;

int currentPosition;
#define IN 0
#define OUT 1
#define FROM_IN_TO_OUT 2
#define FROM_OUT_TO_IN 3
//#define UNKNOWN -1

#define minRailLenght 20 	// Longitud minima del rail en CENTIMETROS
#define maxRailLenght 300	// Longitud máxima del rail en CM

signed char selectedRailLenghtHundreds = 0;
signed char selectedRailLenghtTenths = 0;
signed char selectedRailLenghtUnits = 0;

/*

Definimos las velocidades a las que se mueve el motor
cuando estamos definiendos los puntos de entrada y de salida
en los modos VideoPro y AstroFocus

*/ 

#define speedP_VP 136	// Velocidad x1 Modo Video Pro
#define speedPP_VP 48	// Velocidad x2 Modo Video Pro
#define speedPPP_VP 30 // Velocidad x3 Modo Video Pro

#define speedP_AF 1249//250hz	// Velocidad x1 Modo Astro Focus
#define speedPP_AF 	624// Velocidad x2 Modo Astro Focus
#define speedPPP_AF 312 // Velocidad x3 Modo Astro Focus

#define speedGoToStart 48		// Velocidad del motor para ir al punto de inicio

#define maxSpeedVideoPro 14000 	// Velocidad máxima Modo Video Pro
#define minSpeedVideoPro 2000	// Velocidad mínima Modo Video Pro

unsigned long period; 	// Periodo de delay entre pulsos para modo AstroZoom (no usamos timers)

// Duración máxima permitada de acuerdo a la velocidad mínima establecida
int maxDuration; 
// Duración mínima permitida de acuerdo a la velocidad máxima establecida
int minDuration; 
boolean durationIntervalIsSet=false; // Se tiene ya los valores max y min de duracion o no.
int selectedDuration=0;		// Duración modo Video Pro

#define maxDurationHoursAFMode	8 	// Maximas horas de duración en modo Astro Focus
// Duración del movimiento en modo AstroFocus
unsigned char selectedDurationSecondsUnits=0;
unsigned char selectedDurationSecondsTens=0;
unsigned char selectedDurationMinutesUnits=0;
unsigned char selectedDurationMinutesTens=0;
unsigned char selectedDurationHours=0;


// Modo ida y vuelta en modo astroFocus
// 0: Solo ida
// 1: Ida/Vuelta - IDA
// 2: Ida/vuelta - VUELTA
// Si se ha seleccionado el modo RoudTrip, se convierte en modo ping-pong (bucle infinito)
unsigned char astroRoundTrip=0;


// Modo TimeLapsePRO
// Duracion de la secuencia en base a la duracion del intervalo y de la distancia a mover
char sSequenceDuration[20];

_extern volatile int lastEncoded;
_extern volatile long encoderValue; 
_extern long lastencoderValue;
_extern int lastMSB;
_extern int lastLSB;
int RTmenuItem; // Variable en tiempo real, cambia por interrupciones
int menuItem;   // Variable a utilizar en las funciones.
int lastMenuItem;

//unsigned long SelectButtonPressed;	// Control botón Select
int selectButtonStatus;

/*******************************************************************************************************/
/* Modos timelapses 																					*/
/*******************************************************************************************************/

#define TLMaxDuration 60

// Configuración
typedef struct config {
	unsigned char language;
	bool encoderSpin;
	int railLenght;	// Longitud del rail en mm
} STConfiguracion;

STConfiguracion config;

/* Direcciones EEPROM */
#define eeEncoderSpinAddr 150
#define eeLanguageAddr 	151
#define eeRailLenghtC  	152		// Centenas de cm
#define eeRailLenghtD 	153		// Decenas de cm
#define eeRailLenghtU	154		// Unidades de cm

//#define eeRailLenghtMSB  153

//int railLenght = 0;

_extern int STATE;                  				// Estado 
_extern int PROGRAM_STATE;							// Estado programa
_extern int direction;								// Dirección

_extern unsigned int currentTLInterval;
_extern unsigned int currentTLDistance;
unsigned int currentSMDistance=0; 

typedef struct programParameters {
	unsigned int speed;
	unsigned long intervalDuration; 
	unsigned long motorMoveDuration;
	} STProgramParameters;

//_extern unsigned long motorMoveDuration;
	
_extern STProgramParameters programParameters;

_extern unsigned long timerAnimation;
_extern char timerAnimationItem;      
_extern unsigned long timerProgram;					// Variable a usar en los timers del programa ejecutandose
_extern unsigned long timerShootInterval;		
_extern unsigned long timerMotorMove;
_extern unsigned long timerTriggerPulseWidth;

// Variables para la generacion de la onda cuadrada para pin A4988stepPin
//_extern unsigned long timerStepPulse;					// generar onda cuadrada
//_extern unsigned long periodoOnda;
volatile unsigned int currentStepperSpeed;

_extern char buffer[20];

_extern unsigned int currentNumShots;


// Variables Scroll menus
unsigned char vpIndex=0; // viewPort Index
unsigned char vpOffset=0; //viewPort Offset
unsigned char maxElements=4; // Maximo de elementos en pantalla

/*******************************************************************************************************/
/* Modo Follow Focus 																					*/
/*******************************************************************************************************/

// Definicion de los puntos de enfoque
struct POINT { 
	bool isDefined;
	long steps;
	unsigned char durationTens;	// Duracion
	unsigned char durationUnits;	// Duracion
	unsigned char durationTenths;	// Duracion
} ;
struct POINT focusPoints [] = {
	{false,0,0,0,0},
	{false,0,0,0,0},
	{false,0,0,0,0},
	{false,0,0,0,0},
	{false,0,0,0,0},
	{false,0,0,0,0}
};

unsigned char selectedDurationSecondsTenths=0;

// Cual es el punto que esta siendo definido. Necesario para la funcion de render.
// ya que el punto A no debe ser posible definir su duracion, solo la posicion.
unsigned char currentPointBeingDefined;	

// Variable para indicar que estamos de camino de un punto a otro
// En caso de que paremos el movimiento en medio de un tramo, sabremos que estamos en el "limbo"
bool inLimbo = false;	

// Usamos o no Timer para generar la onda cuadrada del StepperDriver
bool useTimer = false;


