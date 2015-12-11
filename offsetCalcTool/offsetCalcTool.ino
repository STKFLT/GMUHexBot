/*
 * offsetCalcTool
 * Sketch to calculate servo offsets
 * for GMU Robotics club (Fall 2015).
 */
/* Servo header for the 12 servos of the hexBot. */
#include <Servo.h>
/* Array of the 12 servos on the hexBot. */
Servo servos[12];
/* The offsets for the 12 servos in degrees. Used to map natural angles to servo angles. */
/* In the pair patter on leg_i_perp_servo, leg_i_par_servo, leg_i+1_perp_servo, ... .    */
int degree0offset[12] =   {145,   5,  35, 175,  10,   0, 135, 175, 140,  10,  30, 170};
int degree180offset[12] = {  0, 185, 160,  -5, 170, 170,   0,  -5,   0, 185, 160,  -5};

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

//reads for integers over serial and writes that angle to all servos
void loop() {
  int startI = 0;
  while(Serial.available() > 0) {
    int angle = Serial.parseInt();
    for(int i = startI; i < 12; i+=2)
    {
      writeServo( i, angle);
    }
    startI = startI == 0 ? 1:0;
  }
  delay(100);
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

