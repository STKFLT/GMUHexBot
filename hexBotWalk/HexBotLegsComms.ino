/*
 * HexBotLegs.ino
 * Slave communication protocol for the HexBot
 */

#include <Wire.h>

#define LED_PIN 13

char* cmd_arr;
int cmd_len;

/** Setup the connection over I2C. **/
void setupWire()
{
  cmd_arr = NULL;
  cmd_len = 0;
  
  Wire.begin(8);          // Join i2c bus with address #8
  Wire.onReceive(getCMD); // Register event handeler
  
  pinMode(LED_PIN, OUTPUT);   // Initialize the LED pin
  digitalWrite(LED_PIN, LOW); // Turn off the LED
}

/** Run the command if their is one. **/
void runCMD()
{
  if(cmd_len > 0)
    parseCMD(cmd_arr, cmd_len);
}

/**
  * Prints out the received command to serial.
  * @param howMany The number of bytes received.
  */
void printCMD(int howMany)
{
  getCMD(howMany);
 
  for (int i = 0; i < cmd_len; i++)
  {
    Serial.print(cmd_arr[i]);
  }
}

/**
  * Prints out the received command to serial.
  * @param howMany The number of bytes received.
  */
void getCMD(int num)
{
  cmd_arr = new char[Wire.available()+1];
  cmd_len = Wire.available();
  
  byte i = 0;
  while (Wire.available() > 0)
  {
    cmd_arr[i++] = Wire.read(); // receive byte as a character
  }
  
  cmd_arr[cmd_len+1] = 0;
}

/** 
  * Parse and run a command if valid.
  * @param cmd The command to parse.
  * @param len The length of the command
  */
void parseCMD(char* cmd, int len)
{
  /* Get the first three bytes. */
  int start = 0;
  char* threeBytes = nextThreeBytes(cmd, len, start);
  start += 4;
  Serial.print("cmd: ");
  Serial.println(threeBytes);
  
  /* If the comand does not have the header RBT then return. */
  if(strcmp(threeBytes,"RBT") != 0)
    return;

  /* Loop through the commands until an END or no more bytes. */
  while(strcmp(threeBytes, "END") && len-start > 0)
  {
    free(threeBytes);
    threeBytes = nextThreeBytes(cmd, len, start);
    start += 4;
    Serial.print("cmd: ");
    Serial.println(threeBytes);
    
    /* Move the robot. */
    if(strcmp(threeBytes, "RUN") == 0)
    {      
      /* Loop through all movement commands. */
      do
      {
        free(threeBytes);
        threeBytes = nextThreeBytes(cmd, len, start);
        start += 4;
        Serial.print("cmd: ");
        Serial.println(threeBytes);
     
        if(strcmp(threeBytes, "LFT") == 0)
        {
          //servo loop for turn left
          turn(false, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, 
			PIVOT_PITCH_POSITIVE, PIVOT_PITCH_NEGATIVE, WAIT);
        }
        else if(strcmp(threeBytes, "RGT") == 0) 
        {
          //servo loop for turn right
          turn(true, ROLL, PITCH_POSITIVE, PITCH_NEGATIVE, PIVOT_PITCH_POSITIVE,
			PIVOT_PITCH_NEGATIVE, WAIT);
        }
        else if(strcmp(threeBytes, "FWD") == 0) 
        {
          //servo loop for forwards
          forward(true);
        }       
        else if(strcmp(threeBytes, "BWD") == 0 )
        {
          //servo loop for back
          backward(true);
        }
        else
        {
          break;
        }
      }while(true);
    }
    /* If the command is a LED state command. */
    else if(strcmp(threeBytes, "LED") == 0)
    {
      free(threeBytes);
      threeBytes = nextThreeBytes(cmd, len, start);
      start += 4;
      Serial.print("cmd: ");
      Serial.println(threeBytes);
      
      /* Turn the LED on. */
      if(strcmp(threeBytes, "ONN") == 0)
      {
        digitalWrite(LED_PIN, HIGH);
      }
      /* Turn LED off. */
      else if(strcmp(threeBytes,"OFF") == 0)
      {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
  
  free(threeBytes);
  free(cmd_arr);
  cmd_len = 0;
}

/** 
  * Parse and run a command if valid.
  * @param cmd The command to parse.
  * @param len The length of the command
  */
char* nextThreeBytes(char* cmd, int len, int start)
{
  if(len-start < 3)
    return NULL;
  
  char* threeBytes = (char*)calloc(4, sizeof(char));
  for(int i = start; i < 3+start; i++)
  {
    threeBytes[i] = cmd[i];
  }
  
  // read the space
  if(len-start-3 > 0)
    Wire.read();	
  
  return threeBytes;
}
