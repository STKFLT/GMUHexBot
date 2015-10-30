/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>
void setupWire()
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(parseCMD); // register event
}
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void printCMD(int howMany)
{
	char sentArr[Wire.available()];
	byte i = 0;
  
	while (1 < Wire.available()) {
		char c = Wire.read(); // receive byte as a character
		sentArr[i] = c;
		i++;
  	} 
  
	for (int j = 0; j < i; j++)
	{
		Serial.print(sentArr[j]);
	}
  Serial.print("\n");
}

void parseCMD(int num)
{
  char* cmd = nextThreeBytes();
  if(cmd != "RBT")
    return;
  
  while(cmd != "END" && Wire.available() > 2)
  {
    cmd = nextThreeBytes();
    if(cmd == "RUN")
    {
      cmd = nextThreeBytes();
      do
      {
        if(cmd == "LFT")
        {
          turn(false, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, PIVOT_PITCH_POSITIVE, PIVOT_PITCH_NEGATIVE, WAIT);
        }
        else if(cmd == "RGT")
        {
          turn(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, PIVOT_PITCH_POSITIVE, PIVOT_PITCH_NEGATIVE, WAIT);
        }
        else if(cmd == "FWD")
        {
          forward(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);
        }       
        else if(cmd == "BWD")
        {
          backward(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);
        }
        else
        {
          break;
        }
      }while(true);
    }
  }
}

char* nextThreeBytes()
{
  if(Wire.available() < 2)
    return NULL;
  
  char cmd[3];
  for(int i = 0; i < 3; i++)
  {
    cmd[i] = Wire.read();
    Serial.print(cmd[i]);
  }
  // read the space
  if(Wire.available() > 0)
  {
    Wire.read();
  }
  return cmd;
}
