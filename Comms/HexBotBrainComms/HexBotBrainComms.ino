#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(); // join i2c bus (address optional for master)
}

byte x = 0;

void loop() {
  
  char cmd[3];
  if (Serial.available() > 0) {
    // read incoming serial data:
    for (int i = 0; i < 3; i++){
      char inChar = Serial.read();
      cmd[i] = inChar;
    }
  }
  
  Wire.beginTransmission(8); // transmit to device #8
  if (cmd[0] != 0){
    for (int i = 0; i < 3; i++){
      Serial.print(cmd[i]);
    }
    Serial.println();
  }
  Wire.write(cmd);        // sends a number of bytes
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
  
  delay(500);
}
