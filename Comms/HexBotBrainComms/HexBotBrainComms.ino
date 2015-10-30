#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

void loop() {
  
//  char cmd[3];
//  if (Serial.available() > 0) {
//    // read incoming serial data:
//    for (int i = 0; i < 3; i++){
//      char inChar = Serial.read();
//      cmd[i] = inChar;
//    }
//  }
//  if (cmd[0] != 0){
//    for (int i = 0; i < 3; i++){
//      Serial.print(cmd[i]);
//    }
//    Serial.println();
//  }
  
  while(Serial.available() == 0)
  {
    delay(100);
  }
  Wire.beginTransmission(8); // transmit to device #8
  while (Serial.available() > 0){
    char inChar = Serial.read();
    Serial.print(inChar);
    Wire.write(inChar); 
  }
  Serial.println();

  Wire.endTransmission();    // stop transmitting
  
  delay(500);
}
