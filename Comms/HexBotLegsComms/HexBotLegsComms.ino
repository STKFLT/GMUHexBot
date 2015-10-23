/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>

void setup()
{
 	Wire.begin(8);                // join i2c bus with address #8
	Wire.onReceive(parseCmd); // register event
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
	parseCMD(sentArr);
}

void parseCMD(int num)
{
	char[] cmd = nextThreeBytes();
	if(cmd != "RBT")
		return;
	
	while(cmd != "END" && Wire.available > 2)
	{
		cmd = nextThreeBytes();
		if(cmd == "RUN")
		{
			cmd = nextThreeBytes();
			do
			{
				if(cmd == "LFT")
					//servo loop for turn left
				else if(cmd == "RGT")
				else if(cmd == "FWD")
				else if(cmd == "BWD")
				else
					break;
			}while(true);
		}
	}
}

char* nextThreeBytes()
{
	if(Wire.available() < 2)
		return NULL;

	char[] cmd = new char[3];
	for(int i = 0; i < 3; i++)
	{
		cmd[i] = Wire.read();
	}
	// read the space
	if(Wire.available() > 0)
		Wire.read();	

	return cmd;
}
