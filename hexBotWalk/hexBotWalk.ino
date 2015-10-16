/*
 * hexBotWalk
 * Sketch for a hexBot walk cycle.
 * Created for GMU Robotics club (Fall 2015).
 */
/* Servo header for the 12 servos of the hexBot. */
#include <Servo.h>

/* Set constants for the walk loop */
#define ROLL 30
#define PITCH_POSITIVE 80
#define PITCH_NEGATIVE 105
#define WAIT 500

/* Array of the 12 servos on the hexBot. */
Servo servos[12];
/* The offsets for the 12 servos in degrees. Used to map natural angles to servo angles. */
/* In the pair patter on leg_i_perp_servo, leg_i_par_servo, leg_i+1_perp_servo, ... .    */
int degree0offset[12] =   {140,   0,  35, 180, 0/*Broken*/,   0,  45, 180, 140,   0,  30, 180};
int degree180offset[12] = {  0, 180, 160,   0, 0          , 180, 145,   0,   0, 180, 160,   0};

/* Initializes the 12 servos. */
void setup()
{
  Serial.begin(9600);

  /* Loop through the servos. */
  for(int i = 0; i < 12; i++)
  {
    /* Pins start at 2 */
    servos[i].attach(i+2);
    /* Resets the position of the servo to 0 degrees. */
    writeServo(i, i%2 ? 90:0);
  }
  
  delay(1000);
}

/* Loop through the walk cycle. */
void loop()
{
  /* Right step. */
  step(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);
  
  //delay(500);
  
  /* Left step. */
  step(false, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);

}

/* Steps the legs in the triangle specified with the specified angles and wait time. */
/* Param(s): isRight - a boolean for if the step is the right side or left.          */
/*           roll - the roll angle of the steps.                                     */
/*           pitchPositive - the positive pitch angle of the steps.                  */
/*           pitchNegative - the negative pitch angle of the steps.                  */
/*           wait - the amount time in milliseconds to wait between parts.           */
void step(boolean isRight, int roll, int pitchPositive, int pitchNegative, int wait)
{
  /* Gets the starting index of the servos. */
  int legSet = isRight ? 0 : 6;
 
  //roll out
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i, roll);
  }
  delay(wait);
  
  //pitch forward
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i+1, pitchPositive);
  }
  delay(wait);
   
  //roll in
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i, 0);
  }
  delay(wait);
  
  //pitch backward
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i+1, pitchNegative);
  }
  delay(wait);
  
  Serial.println("Step Completed");
}


/* Converts the desired degree to the servo speified's actual angle of rotation. */
/* Param(s): servo - the index into the servo array for the servo desired.       */
/*           degree - the angle for which the servo should be moved to.          */
void writeServo(int servo, int degree)
{
  /* Write to the servo the exact degree that is converted from the given one. */
  servos[servo].write(map(degree, 0, 180, degree0offset[servo], degree180offset[servo]));

  /* Log debug information. */
  char debug_log[32];
  sprintf(debug_log, "Writing %d to Servo #%d\n", degree, servo);
  Serial.print(debug_log);
}

