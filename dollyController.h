  
extern void drawRTM(void);
extern void drawRVM(void); 
void resetEncoder (void	);
void processEvents(void);   
extern int checkCollision(void);  
void draw(void);
void processSelectButton(void);
void selectButtonPressed (void);
void checkDirection (void);

void processEncoderRotation(void);
void makeSelectionCiclyc (void);
void processEncoderPlusClick (void);
void processEncoderMinusClick (void);

                          
/* Rotary Encoder Functions */
extern void updateEncoder (void);
extern void rotaryEncoderSetup (void);
void disableEncoder(void);
void enableEncoder(void);

/* Rendering Functions */ 
extern void u8gsetup();
extern int u8gnextpage(void);
extern void u8gfirstpage(void);
extern void drawMenu(char *[], int);
extern void drawTimeLapse(void);
extern void drawTimeLapsePro(void);
extern void drawVideo(void);      
extern void drawChangeLanguage(void);  
extern void renderRunningFooter(void);
extern void renderTitle(char *,int, int);
extern void renderAnimation(void);     
extern void renderSelectCursor (int, int, int, int);    
extern void renderAnimationRuler(void);  
extern void renderBoxAroundText (int, int, char*);    
extern void drawSplashScreen(void);
extern void renderVersionNumber (void);

void calculateTimeLapseParameters(void);

/* Program running */
void timeLapseProgramWillRun(void);
void timeLapseProgramWillStop(void);
void timeLapseProProgramWillRun(void);
//void runTimeLapseProgram(void);  
void timeLapseProgramIsRunning(void);  

void videoProgramIsRunning(void);
void videoProgramWillRun(void);
void videoProgramWillStop(void);
//void stopProgram(int);

void stopMotionProgramWillRun(void);

extern void initMotor(void);
extern void stopMotor(void);   
extern void moveDCMotor (int);
//extern void moveStepperMotor (int);
void configureMicrosteping (char);

//void motorWillMove(void);
//void motorIsMoving(void);
//void motorWillStop(void);
void moveMotorSteps (unsigned int);

unsigned int convertPWM2TimerRegister (int);
//unsigned long convertSpeed (int);
unsigned char ramp (unsigned char);
//unsigned long rampStepper (unsigned long);

void disableTimer1Interrupt(void);
void enableTimer1Interrupt(void);
void enableTimer1InterruptForVideoMode(void);

void disableTimer2Interrupt(void);
void enableTimer2Interrupt(void);
void changeMenuItem (int);
void changeEncoderSpin(void);
void changeLanguage(int);
void readConfigFromEEPROM (void);

void pointIsBeingDefined(void);
void pointDefineWillRun(void);
void inPointIsDefined(void);
void outPointIsDefined(void);
void defineDurationInterval (void);
void motorWillMoveToStartPosition(void);
void motorIsMovingToStartPosition (void);
void videoProModeIsRunning (void);
void videoProModeWillRun (void);
void calculateVideoProParameters (void);
void initializeVideoProMode(void);

void pingPongModeWillRun (void);
void pingPongModeIsRunning (void);

char validateAndSaveRailLenght2EEPROM (void);

