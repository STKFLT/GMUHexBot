/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>

void setup()
{
 	Wire.begin(8);                // join i2c bus with address #8
	Wire.onReceive(parseCMD); // register event
	Serial.begin(9600);           // start serial for output
}

void loop()
{
	delay(100);
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
          //servo loop for turn left
        }
        else if(cmd == "RGT")
        {
          //servo loop for turn right
        }
        else if(cmd == "FWD")
        {
          //servo loop for forwards
        }       
        else if(cmd == "BWD")
        {
          //servo loop for back
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
//This function reads three bytes from the stream, right? It oughta return NULL when less than 3.
//If Wire.available() = 2, it'll still try to read it. Unacceptabllllllle! --Robert (ComradePayne)
  //if(Wire.available() < 2)
  if(Wire.available() < 3)
    return NULL;
  
  char cmd[3];
  for(int i = 0; i < 3; i++)
  {
    cmd[i] = Wire.read();
  }
  // read the space
  if(Wire.available() > 0)
    Wire.read();	
  
  return cmd;
}
