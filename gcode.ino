#include <String.h>
#include "processor.h"
#include "help.h"


#define BAUD 57600 //define speed of communication
#define MAX_BUF 64 //max length of message arduino can store

char buffer[MAX_BUF];
int sofar=0, i=0; //how much is in the buffer

void setup() 
{
  Serial.begin(BAUD);
  help();
  sready();
}

/**
 * After setup() this machine will repeat loop() forever.
 */
void loop() 
{
  // listen for commands
  while( Serial.available() ) 
  { 
	// if something is available
    char c = Serial.read(); // get it
    Serial.print(c); // optional: repeat back what I got for debugging
		
    // store the byte as long as there's room in the buffer.
    // if the buffer is full some data might get lost
    if(sofar < MAX_BUF) 
		buffer[sofar++]=c;
	else
	{
		Serial.print(F("Buffer Overflow!!"));
		sready();
		break;
	}
	
	// if we got a return character (\n) the message is done.
	if(c==';')
	{
      Serial.print(F("\r\n")); // optional: send back a return for debugging

      // strings must end with a \0.
      buffer[sofar-1]=0;
      String cmd = String(buffer);
      processCommand(cmd); // do something with the command
      sready();
    }
  }
}

void sready()
{
  sofar=0;
  Serial.println(F("> "));
}

