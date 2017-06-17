#include <String.h>
#include "processor.h"

#define BAUD 57600 //define speed of communication
#define MAX_BUF 64 //max length of message arduino can store
#define VERSION 0.1

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


/**
 * display helpful information
 */
void help() {
  Serial.print(F("CNC Robot "));
  Serial.println(VERSION);
  Serial.println(F("Commands:"));
  Serial.println(F("G00 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));
  Serial.println(F("G01 [X(steps)] [Y(steps)] [F(feedrate)]; - linear move"));
  Serial.println(F("G04 P[seconds]; - delay"));
  Serial.println(F("G90; - absolute mode"));
  Serial.println(F("G91; - relative mode"));
  Serial.println(F("G92 [X(steps)] [Y(steps)]; - change logical position"));
  Serial.println(F("M18; - disable motors"));
  Serial.println(F("M100; - this help message"));
  Serial.println(F("M114; - report position and feedrate"));
}
/**
 * prepares the input buffer to receive a new message and 
 * tells the serial connected device it is ready for more.
 */
void sready() 
{
  sofar=0; // clear input buffer
  Serial.print(F("> ")); // signal ready to receive input
}


