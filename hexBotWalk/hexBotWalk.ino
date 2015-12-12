/*
 * hexBotWalk
 * Sketch for a hexBot walk cycle.
 * Created for GMU Robotics club (Fall 2015).
 * Comment Editor: ComradePayne
 */

/* Servo header for the 12 servos of the hexBot. */
#include <Servo.h>

#define DEBUG 0
#define COMMS 0

/* Set constants for the walk loop */
#define ROLL  30
#define ROLL_IN 0
#define PITCH_POSITIVE 80
#define PITCH_NEGATIVE 105
#define PIVOT_PITCH_POSITIVE 80
#define PIVOT_PITCH_NEGATIVE 95
#define WAIT 50

/* Array of the 12 servos on the hexBot. */
Servo servos[12];
/* The offsets for the 12 servos in degrees. Used to map natural angles to servo angles. */
/* In the pair pattern on leg_i_perp_servo, leg_i_par_servo, leg_i+1_perp_servo, ... .   */
/*                          0    1    2    3    4    5    6     7    8    9   10   11    */
int degree0offset[12]   = {145,   5,  35, 175,  10,   0, 135, 175, 140,  10,  30, 170};
int degree180offset[12] = {  0, 185, 160,  -5, 170, 170,   0,  -5,   0, 185, 160,  -5};

/* Initializes the 12 servos. */
void setup()
{
  #if COMMS
    /* Set up the I2C code. */
    setupWire();
  #endif

  #if DEBUG
    /* Begin serial communications. */
    Serial.begin(9600);
  #endif

  /* Loop through the servos. */
  for(int i = 0; i < 12; i++)
  {
    /* Pins start at 2 */
    servos[i].attach(i+2);
    /* Resets the position of the servo to 0 degrees. */
    writeServo(i, i%2 ? 90:ROLL_IN);
  }
}

/* Loop through the walk cycle. */
void loop()
{
  #if COMMS
    /* Run the command received over I2C. */
    runCMD();  
  #else
    /* Right step. */
    forward(true);
//    turn(false, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, 90, 90, WAIT);
//    strafe(true, ROLL, WAIT+50);
    delay(WAIT);
    
    /* Left step. */
    forward(false);
  #endif
  
  delay(WAIT);
}

/**
  * Steps the legs forward in the triangle specified with the specified angles 
  * and wait time.
  * @param isRight A boolean for if the step is the right side or left.
  */
void forward(boolean isRight)
{
  step(isRight, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE,  WAIT);
}

/**
  * Steps the legs backward in the triangle specified with the specified angles 
  * and wait time.
  * @param isRight A boolean for if the step is the right side or left.
  */
void backward(boolean isRight)
{
  step(isRight, ROLL, PITCH_NEGATIVE, PITCH_POSITIVE,  WAIT);
}

/**
  * Steps the legs in the triangle specified with the specified angles and wait 
  * time.
  * @param isRight A boolean for if the step is the right side or left.
  * @param roll The roll angle of the steps.
  * @param pitchPositive The positive pitch angle of the steps.
  * @param pitchNegative - the negative pitch angle of the steps.
  * @param wait The amount time in milliseconds to wait between parts.
  */
void step(boolean isRight, int roll, int pitchPositive, int pitchNegative, 
  int wait)
{
  turn(isRight, roll, pitchPositive, pitchNegative, pitchPositive, pitchNegative, wait);
}

/**
  * Turn the hexBot either right or left. In theory, we cause the robot to bear 
  * left or right (depending on isRight's value) by designating the tip of the 
  * respective triangle of legs as the "pivot leg". This pivot leg would have a 
  * shorter stride than the two other legs, causing the robot to slowly turn 
  * itself right or left, depending on which side the pivot leg is on.
  * @param isRight A boolean for if the step is the right side or left.
  * @param roll The roll angle of the steps.
  * @param movePitchPositive Designates the "normal" stride length. 
  * @param movePitchNegative Designates the "normal" stride length. . 
  * @param pivotPitchPositive The special, smaller pitch variable, representing 
  *   a shorter stride length.
  * @param pivotPitchNegative The special, smaller pitch variable, representing 
  *   a shorter stride length.
  * @param wait - Length of time (in milliseconds) to delay before, in higher 
  *   level functions, the opposite set of legs is moved.
  */
void turn(boolean isRight, int roll, int movePitchPositive, 
  int movePitchNegative, int pivotPitchPositive, int pivotPitchNegative,
  int wait)
{
  /* Gets the starting index of the servos. */
  int legSet = isRight ? 0 : 6;
 
  //roll out
  writeServo(legSet, roll);
  if(pivotPitchPositive != pivotPitchNegative){
    writeServo(legSet + 2, roll);
  }
  writeServo(legSet + 4, roll);
  delay(wait);

  writeServo(legSet + 1, movePitchPositive);
  writeServo(legSet + 3, pivotPitchPositive);
  writeServo(legSet + 5, movePitchPositive);
 
  delay(wait);
   
  //roll in
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i, ROLL_IN);
  }
  delay(wait);
  
  writeServo(legSet + 1, movePitchNegative);
  writeServo(legSet + 3, pivotPitchNegative);
  writeServo(legSet + 5, movePitchNegative);
  
  delay(wait);
  
  Serial.println("Step Completed");
}

/** 
  * Strafes a single step to the left/right, depending on IsRight's value. Uses 
  * the roll and wait values we defined earlier in this sketch.
  * @param isRight Is this strafe a right strafe.
  * @param roll The roll angle for the strafe.
  * @param wait The wait time between movements. 
  */ 
void strafe(boolean isRight, int roll, int wait)
{
  int rowLeftIndices[]  = {0, 8,  4};
  int rowRightIndices[] = {6, 2, 10};
  int *StartingRow, *FollowingRow;
  
  StartingRow = isRight ? rowRightIndices: rowLeftIndices;
  FollowingRow = isRight ? rowLeftIndices: rowRightIndices;
  
  /*
   Begin strafe procedure:
   1. Move starting legs out.
   2. Move starting legs in, dragging bot with it.
   3. Move following legs in, poised to extend.
   4. Extend following legs to 0 degrees.
   5. Slightly afterwards, extend starting legs to 0 degrees, completing the 
      step.
  */

  for(int i = 0; i < 3; i++){
    writeServo(StartingRow[i], roll);
  }
  
  delay(wait);
  
  for(int i = 0; i< 3; i++){
    writeServo(StartingRow[i], 0);
  }
    
  delay(wait);
  
  for(int i = 0; i< 3; i++){
    writeServo(FollowingRow[i], -roll);
  }
  
  delay(wait);
  
  for(int i =0; i< 3; i++){
    writeServo(FollowingRow[i], 0);
  }
  
  delay(wait);
 }
 
/** 
  * Converts the desired degree to the servo specified's actual angle of 
  * rotation.
  * @param servo The index into the servo array for the servo desired.
  * @param degree The angle for which the servo should be moved to.
  */
void writeServo(int servo, int degree)
{
  /* Write to the servo the exact degree that is converted from the given */
  /* one.                                                                 */
  servos[servo].write(map(degree, 0, 180, degree0offset[servo], 
    degree180offset[servo]));

  /* Log debug information. */
  #if DEBUG
    char debug_log[32];
    sprintf(debug_log, "Writing %d to Servo #%d\n", degree, servo);
    Serial.print(debug_log);
  #endif
}
