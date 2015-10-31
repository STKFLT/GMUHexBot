/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>
#define LED_PIN 13

void setupWire()
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(parseCMD); // register event
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void printCMD(int howMany)
{
	char sentArr[Wire.available()];
	byte i = 0;
  
	while (Wire.available() > 0) {
		char c = Wire.read(); // receive byte as a character
		sentArr[i++] = c;
  	} 
  
	for (int j = 0; j < i; j++)
	{
		Serial.print(sentArr[j]);
	}
}

void parseCMD(int num)
{
  char* cmd = nextThreeBytes();
  Serial.print("cmd: ");
  Serial.println(cmd);
  if(strcmp(cmd,"RBT") != 0)
    return;
  
  while(strcmp(cmd, "END") && Wire.available() > 0)
  {
    cmd = nextThreeBytes();
    Serial.print("cmd: ");
    Serial.println(cmd);
  
    if(strcmp(cmd, "RUN") == 0)
    {
      cmd = nextThreeBytes();
      do
      {
        if(strcmp(cmd, "LFT") == 0)
        {
          //servo loop for turn left
	  turn(false, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, PIVOT_PITCH_POSITIVE, PIVOT_PITCH_NEGATIVE, WAIT);
       }
        else if(strcmp(cmd, "RGT") == 0) 
        {
          //servo loop for turn right
          turn(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, PIVOT_PITCH_POSITIVE, PIVOT_PITCH_NEGATIVE, WAIT);
        }
        else if(strcmp(cmd, "FWD") == 0) 
        {
          //servo loop for forwards
          forward(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);
        }       
        else if(strcmp(cmd, "BWD") == 0 )
        {
          //servo loop for back
          backward(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, WAIT);
        }
        else
        {
          break;
        }
      }while(true);
    }
    else if(strcmp(cmd, "LED") == 0)
    {
	cmd = nextThreeBytes();
        
        if(strcmp(cmd, "ONN") == 0)
	{
		digitalWrite(LED_PIN, HIGH);
	}
	else if(strcmp(cmd,"OFF") == 0)
	{
		digitalWrite(LED_PIN, LOW);
	}
    }
  }
}

char* nextThreeBytes()
{
  if(Wire.available() < 3)
    return NULL;
  
  char* cmd = (char*)calloc(4, sizeof(char));
  for(int i = 0; i < 3; i++)
  {
    cmd[i] = Wire.read();
  }
  
  // read the space
  if(Wire.available() > 0)
    Wire.read();	
  
  return cmd;
}
