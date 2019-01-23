//#ifndef _MAINFILE
const uint8_t arrow_noSelect_bitmap[] U8G_PROGMEM = {
	0xff,0xfe,0x80,0x2,0x82,0x2,0x86,0x2,0x8e,0x2,0x9f,0xfa,0xbf,0xfa,0x9f,0xfa,
	0x8e,0x2,0x86,0x2,0x82,0x2,0x80,0x2,0xff,0xfe,0x0,0x0,0x0,0x0,0x0,0x0
};


const uint8_t arrow_select_bitmap[] U8G_PROGMEM = {
	0xff,0xfe,0xff,0xfe,0xfd,0xfe,0xf9,0xfe,0xf1,0xfe,0xe0,0x6,0xc0,0x6,0xe0,0x6,
	0xf1,0xfe,0xf9,0xfe,0xfd,0xfe,0xff,0xfe,0xff,0xfe,0x0,0x0,0x0,0x0,0x0,0x0
};
//#endif


// Literales generales
/***********************************************************************************/
//#ifndef _MAINFILE  // incluimos solo en render.cpp

char *strShots[LANGUAGES] = {"DISPAROS","SHOTS"};
char *strRun[LANGUAGES] = {"EMPEZAR","RUN"};
char *strMode[LANGUAGES] = {"MODO", "MODE"};   
char *strRunning[LANGUAGES] = {"Ejecutando", "Running"};
char *strSpeed[LANGUAGES] = {"Velocidad","Speed"}; 
char *strRailLenght [LANGUAGES] = {"Longitud rail", "Rail lenght"};
//#endif

//Estructuras con los literales de los menus y pantallas
/***********************************************************************************/

typedef struct  {
	int arrayLen;
	char *str[LANGUAGES][10];
	} STMenuLevel0;

#ifdef _MAINFILE
STMenuLevel0 menuLevel0 ={10,{{"Modo Video", "Modo Video Pro","Modo TimeLapse", "Modo TimeLapse Pro" , "Modo StopMotion", "Modo AstroFocus", "Modo FollowFocus", "Activar freno", "Liberar freno", "Configuracion" },
			{"Video mode", "Pro Video Mode", "TimeLapse mode", "TimeLapse Pro mode", "StopMotion Mode", "AstroFocus Mode", "FollowFocus Mode","Brake","Release brake","Configuration"}}};		 
#else
		extern STMenuLevel0 menuLevel0;
#endif
/***********************************************************************************/	
typedef struct  {
	int arrayLen;
	char *str[LANGUAGES][4];
	} STMenuLevel11;
#ifdef _MAINFILE
STMenuLevel11 menuLevel11 = {4, {{"..Volver", "Periodo", "Desplazamiento", "Run"},
							{"..Back", "Period", "Displacement", "Run"}}};
#else
extern STMenuLevel11 menuLevel11;
#endif
/***********************************************************************************/
typedef struct {
	int arrayLen;
	char *str[LANGUAGES][2];
	} STMenuLevel12;

#ifdef _MAINFILE
	STMenuLevel12 menuLevel12  = {2, {{"..Volver", "Idioma",},{"..Back", "Language",}}};
#else
extern STMenuLevel12 menuLevel12;
#endif

/***********************************************************************************/                             
typedef struct  {
	char *strTitle[LANGUAGES];
	int hasBackButton;
	int numElements;
	char *strElement[LANGUAGES][3];
	char *strLanguages[LANGUAGES];
	
	int numInvertSelectElements;
	char *strInvertSelect[LANGUAGES];

	} STmenuConfiguration;

#ifdef _MAINFILE
STmenuConfiguration menuConfiguration  = {
										{"Configuracion","Configuration"},
										1,
										3,
										{{"Idioma:", "Invertir Select:", "Longitud rail:"}, {"Language:", "Invert Select:", "Rail lenght:"}},	
										{"Espa\xF1ol", "English"},
										2,
										{"<<<", ">>>"},
									};
#else
extern STmenuConfiguration menuConfiguration;
#endif 										

/***********************************************************************************/     
// Modo TimeLapse
/***********************************************************************************/                             
typedef struct  {
	char hasBackButton;
	int strLen;  
	char *str[LANGUAGES][2];
	int strIntervalLen;
	unsigned int interval[9];   // En milisegundos
	char *strInterval[9];
	char *strTitle[LANGUAGES];  
	} STmenuTimeLapse;

#ifdef _MAINFILE
STmenuTimeLapse menuTimeLapse =  	{1,
									2,
									{{"INTERVALO","EMPEZAR"},{"INTERVAL","RUN"}},
								   	9,
								 	{1000,2000,3000,4000,6000,8000,10000,20000,30000}, 
									{"1s", "2s", "3s", "4s", "6s", "8s", "10s", "20s", "30s"},
									{"Modo TimeLapse", "TimeLapse mode"}};
#else
extern STmenuTimeLapse menuTimeLapse;
#endif 										

/***********************************************************************************/                             
// Modo TimeLapsePro
/***********************************************************************************/                             
// typedef struct  {                   
// 	char hasBackButton;
// 	int strLen;    
// 	char * str[LANGUAGES][3];
// 	int strIntervalLen;
// 	double interval[9];   // En milisegundos
// 	char *strInterval[9];
// 	int strDistanceLen;
// 	double distance[9];
// 	char *strDistance[LANGUAGES][9];
// 	char *strTitle[LANGUAGES];  
// 	}  STnewMenuTimeLapsePro;
// #ifdef _MAINFILE 
// STnewMenuTimeLapsePro newMenuTimeLapsePro =  {1, 
// 	3, {{"SECUENCIA","TIMELAPSE","EMPEZAR"}, {"Sequence","Timelapse","RUN"}}, 
// 	9,
//  	{1000,2000,3000,4000,6000,8000,10000,20000,30000}, 
// 	{"1s", "2s", "3s", "4s", "6s", "8s", "10s", "20s", "30s"},
// 	9, {64,128,200,210,220,230,240,250,255}, {{"64","128","200","210","220","230","240","250","255"}, {"64","128","200","210","220","230","240","250","255"}},
// 	{"TimeLapse Pro", "TimeLapse Pro"}};  
// #else
// extern STnewMenuTimeLapsePro newMenuTimeLapsePro;
// #endif	

/***********************************************************************************/                             
// Modo TimeLapsePro
/***********************************************************************************/                             
typedef struct  {                   
	char hasBackButton;
	int strLen;    
	char * str[LANGUAGES][3];
	int strIntervalLen;
	double interval[9];   // En milisegundos
	char *strInterval[9];
	int strDistanceLen;
	double distance[10]; // Numero de pasos
	char *strDistance[LANGUAGES][10];
	char *strTitle[LANGUAGES];  
	}  STmenuTimeLapsePro;
#ifdef _MAINFILE 
STmenuTimeLapsePro menuTimeLapsePro =  {1, 
	3, {{"INTERVALO","DISTANCIA","EMPEZAR"}, {"INTERVAL","DISTANCE","RUN"}}, 
	9,
 	{1000,2000,3000,4000,6000,8000,10000,20000,30000}, 
	{"1s", "2s", "3s", "4s", "6s", "8s", "10s", "20s", "30s"},
	10, {80,160,240,320,400,480,560,640,720,800}, 
	{{"0,25mm","0,5mm","0,75mm","1mm","1,25mm","1,5mm","1,75mm","2mm","2,25mm","2,5mm"}, 
	{"0,25mm","0,5mm","0,75mm","1mm","1,25mm","1,5mm","1,75mm","2mm","2,25mm","2,5mm"}},
	{"TimeLapse Pro", "TimeLapse Pro"}};  
#else
extern STmenuTimeLapsePro menuTimeLapsePro;
#endif	

/***********************************************************************************/                             
// Modo Video
/***********************************************************************************/                             
// typedef struct {
// 	int arrayLen;
// 	unsigned int speed[10];
// 	char *str[10];
// 	} STMenuVideo;
// #ifdef _MAINFILE	
// STMenuVideo menuVideo =  	{10,{1964,136,71,48,37,31,25,22,19,18}, 
// 							{"10%","20%","30%","40%","50%","60%", "70%", "80%", "90%", "100%"}};
// #else
// extern STMenuVideo menuVideo;
// #endif

/***********************************************************************************/                             
// Modo Video
/***********************************************************************************/                             
typedef struct {
	int arrayLen;
	unsigned int speed[10];
	char *str[10];
	} STMenuVideo;
#ifdef _MAINFILE	

STMenuVideo menuVideo =  	{10,{278,136,71,48,37,31,25,22,19,18}, 
							{"10%","20%","30%","40%","50%","60%", "70%", "80%", "90%", "100%"}};
#else
extern STMenuVideo menuVideo;
#endif

/***********************************************************************************/                             
// Modo Video PRO
/***********************************************************************************/                             
typedef struct  {
	char hasBackButton;
	int numElements;  
	char *str[LANGUAGES][6];
	char *strTitle[LANGUAGES];  
	} STmenuVideoPro;

#ifdef _MAINFILE
STmenuVideoPro menuVideoPro = {1,
								5, 
								{{"Inicio","Final","Duraci\xF3n","Empezar", "Ir inicio", "PingPong"},{"Start","End","Duration", "Run", "Goto Start",  "PingPong"}},
								{"Modo Video Pro", "Video Pro Mode"}
								};
#else
extern STmenuVideoPro menuVideoPro; 
#endif 			

/***********************************************************************************/     
// Modo StopMotion
/***********************************************************************************/                             
typedef struct  {
	char hasBackButton;
	int strLen;  
	char *str[LANGUAGES][2];
	int strDistanceLen;
	unsigned int distance[9];
	char *strDistance[9];
	char *strTitle[LANGUAGES];  
	} STmenuStopMotion;

#ifdef _MAINFILE
STmenuStopMotion menuStopMotion = {1,
									2,
									{{"DISTANCIA","DISPARAR"},{"DISTANCE","SHOT"}},
								   	9,
								 	{5,10,15,20,25,30,35,40,45}, 
									{"0.5mm", "1mm", "1.5mm", "2mm", "2.5mm", "3mm", "3.5mm", "4mm", "4.5mm"},
									{"Modo StopMotion", "StopMotion mode"}};
#else
extern STmenuStopMotion menuStopMotion; 
#endif 		

/***********************************************************************************/                             
// Modo Astro Focus
/***********************************************************************************/                             
typedef struct  {
	char hasBackButton;
	int numElements;  
	char *str[LANGUAGES][5];
	char *strTitle[LANGUAGES];  
	} STmenuAstroFocus;

#ifdef _MAINFILE
STmenuAstroFocus menuAstroFocus = {1,
								4, 
								{{"Inicio","Final","Duraci\xF3n","Empezar", "Ir inicio"},{"Start","End","Duration", "Run", "Goto Start"}},
								{"Modo AstroFocus", "AstroFocus Mode"}
								};
#else
extern STmenuAstroFocus menuAstroFocus; 
#endif 			

/***********************************************************************************/                             
// Modo Follow Focus
/***********************************************************************************/                             
