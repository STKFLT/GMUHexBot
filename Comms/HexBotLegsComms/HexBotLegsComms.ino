/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>
#define LED_PIN 13

void setup()
{
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(parseCMD); // register event
  Serial.begin(9600);           // start serial for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
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
  { 
    Serial.println("Returning"); 
    return;
  }
  while(strcmp(cmd, "END") && Wire.available() > 0)
  {
    cmd = nextThreeBytes();
    Serial.print("cmd: ");
    Serial.println(cmd);
  
    if(strcmp(cmd, "RUN") == 0)
    {
      do
      {
        cmd = nextThreeBytes();
        if(strcmp(cmd, "LFT") == 0)
        {
          //servo loop for turn left
        }
        else if(strcmp(cmd, "RGT") == 0) 
        {
          //servo loop for turn right
        }
        else if(strcmp(cmd, "FWD") == 0) 
        {
          //servo loop for forwards
        }       
        else if(strcmp(cmd, "BWD") == 0 )
        {
          //servo loop for back
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
      digitalWrite(LED_PIN, LOW);	      }
    }
  }
  Serial.println("Returning");
}

char* nextThreeBytes()
{
//This function reads three bytes from the stream, right? It oughta return NULL when less than 3.
//If Wire.available() = 2, it'll still try to read it. Unacceptabllllllle! --Robert (ComradePayne)
  //if(Wire.available() < 2)
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
