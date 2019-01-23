


#define TRUE 1
#define FALSE 0

/* Pololu A4988 Stepper driver */  
// #define A4988dirPin    24 
// #define A4988ms1Pin    14
// #define A4988ms2Pin    12
// #define A4988ms3Pin    13
// #define A4988enablePin 25  
// #define A4988stepPin  2    

//! \brief Status flags
struct GLOBAL_FLAGS{
  //! True when stepper motor is running.
unsigned char running : 1;
  //! True when uart has received a string (ended with '/n').
unsigned char cmd : 1;
  //! Dummy bits to fill up a byte.
unsigned char dummy : 6;
};


typedef struct {
  //! What part of the speed ramp we are in.
  unsigned char run_state : 3;
  //! Direction stepper motor should move.
  unsigned char dir : 1;
  //! Peroid of next timer delay. At start this value set the accelration rate.
  unsigned long step_delay;
  //! What step_pos to start decelaration
  unsigned long decel_start;
  //! Sets deceleration rate.
  unsigned long decel_val;
  //! Minimum time delay (max speed)
  signed int min_delay;
  //! Counter used when accelerateing/decelerateing to calculate step_delay.
  signed int accel_count;
} speedRampData;

// DEBUG
// Para comprobar si se ejecuta toda la ISR
//int sonda=0;
//volatile unsigned long d[600];
//volatile unsigned int r_step_count = 0;  


//volatile unsigned int step_count = 0;

struct GLOBAL_FLAGS status = {FALSE, FALSE, 0};

/*! \Brief Frequency of timer1 in [Hz].
 *
 * Modify this according to frequency used. Because of the prescaler setting,
 * the timer1 frequency is the clock frequency divided by 8.
 */
// Timer/Counter 1 running on 8,000MHz / 8 = 1000kHz (1uS). (T1-FREQ 1000000)
#define T1_FREQ 312500  // 20Mhz / 64 = 312500Hz

//! Number of (full)steps per round on stepper motor in use.
#define SPR 3200



// Maths constants. To simplify maths when calculating in moveWithRamp().
//#define ALPHA (2*3.14159/SPR)                    // 2*pi/spr
#define ALPHA 0.00196349
#define A_T_x100 ((long)(ALPHA*T1_FREQ*100))     // (ALPHA / T1_FREQ)*100
//#define A_T_x100 ((long)(ALPHA*T1_FREQ))     // (ALPHA / T1_FREQ)*100

#define T1_FREQ_148 ((int)((T1_FREQ*0.676)/100)) // divided by 100 and scaled by 0.676
#define A_SQ (long)(ALPHA*2*10000000000ULL)         // ALPHA*2*10000000000
#define A_x20000 (int)(ALPHA*20000)              // ALPHA*20000
// #define ALPHA 0.0019634                   // 2*pi/spr
// #define A_T_x100 628318     // (ALPHA / T1_FREQ)*100
// #define T1_FREQ_148 6760 // divided by 100 and scaled by 0.676
// #define A_SQ 125663600        // ALPHA*2*10000000000
// #define A_x20000 125.6636             // ALPHA*20000

// Speed ramp states
#define STOP  0
#define ACCEL 1
#define DECEL 2
#define RUN   3

speedRampData srd;
int stepPosition = 0;

// Direction of stepper motor movement
#define CW  0
#define CCW 1

unsigned long sqrt_Taylor(unsigned long);
