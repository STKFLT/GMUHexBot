/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */
#include <Wire.h>

#define LED_PIN 13

/**
  * Setup the connection over I2C.
  */
void setupWire()
{
  Wire.begin(8);              // Join i2c bus with address #8
  Wire.onReceive(parseCMD);   // Register event handeler
  pinMode(LED_PIN, OUTPUT);   // Initialize the LED pin
  digitalWrite(LED_PIN, LOW); // Turn off the LED
}

/**
  * Prints out the recieved command to serial.
  * @param howMany The number of bytes recieved.
  */
void printCMD(int howMany)
{
  char sentArr[Wire.available()];
  byte i = 0;
    
  while (Wire.available() > 0)
  {
    char c = Wire.read(); // receive byte as a character
    sentArr[i++] = c;
  } 
  
  for (int j = 0; j < i; j++)
  {
    Serial.print(sentArr[j]);
  }
}

/** 
  * Parse and run a command if valid.
  * @param num The number of bytes.
  */
void parseCMD(int num)
{
  /* Get the first three bytes. */
  char* cmd = nextThreeBytes();
  Serial.print("cmd: ");
  Serial.println(cmd);
  
  /* If the comand does not have the header RBT then return. */
  if(strcmp(cmd,"RBT") != 0)
    return;

  /* Loop through the commands until an END or no more bytes. */
  while(strcmp(cmd, "END") && Wire.available() > 0)
  {
    cmd = nextThreeBytes();
    Serial.print("cmd: ");
    Serial.println(cmd);
    
    /* Move the robot. */
    if(strcmp(cmd, "RUN") == 0)
    {
      cmd = nextThreeBytes();
      
      /* Loop through all movement commands. */
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
    /* If the command is a LED state command. */
    else if(strcmp(cmd, "LED") == 0)
    {
      cmd = nextThreeBytes();
      
      /* Turn the LED on. */
      if(strcmp(cmd, "ONN") == 0)
      {
        digitalWrite(LED_PIN, HIGH);
      }
      /* Turn LED off. */
      else if(strcmp(cmd,"OFF") == 0)
      {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}

/**
  * Gets the next three bytes from the I2C connection.
  * @returns Returns a char* for the command.
  */
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
