#include <Servo.h>

Servo servos[12];
//float perpOffsets[6] = {45,140,40,135,35,180};
//float parOffsets[6] = {90,-95,110,-100,90,-90};
int degree0offset[12] =   {140,   0,  35, 180, 0/*Broken*/,   0,  45, 180, 140,   0,  30, 180};
int degree180offset[12] = {  0, 180, 168,   0, 0          , 180, 145,   0,   0, 180, 160,   0};

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < 12; i++)
  {
    servos[i].attach(i+2);
    writeServo(i, i%2 ? 90:0);
  }
  
  delay(1000);
}

void loop() {
  step(true, 30, 80, 105, 500);
  //delay(500);
  step(false, 30, 80, 105, 500);

}

void step(boolean right,int roll, int pitchPositive, int pitchNegative, int wait) {
  int legSet = right ? 0 : 6;
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
  
  for(int i = legSet; i < legSet + 6; i = i + 2){
    writeServo(i+1, pitchNegative);
  }
  delay(wait);
  Serial.println("Step Completed");
  
}

void writeServo(int servo, int degree){
  servos[servo].write(map(degree, 0, 180, degree0offset[servo], degree180offset[servo]));
  Serial.write("Writing ");
  Serial.print(degree);
  String output = " to Servo #";
  output = output + servo;
  Serial.println(output);
}

