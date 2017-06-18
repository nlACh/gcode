#include <Stepper.h>
#include <String.h>

//version number
#define VERSION 0.5

//steps per revolution SPR of motors used
//change this when changing motors
#define SPR 64

#define BAUD 57600 //define speed of communication
#define MAX_BUF 64 //max length of message arduino can store

char buffer[MAX_BUF];
int sofar=0, i=0; //how much is in the buffer

Stepper sX(SPR, 2, 4, 3, 5);
Stepper sY(SPR, 8, 10, 9, 11);

struct point
{
  float X;
  float Y;
  int F;
};

struct index
{
	int cx;
	int cy;
	int cf;
};

point P1;

/**P2 will store the previous values
steps required will be calculated w.r.t P2
P2 initialized with 0,0,0 */
point P2;

/**finding the index of X and Y and F in GCODE command
better organization is possible with structs apparently
but the data structure can/may change... */
struct index in1;

//this setup will run only once
void setup() 
{
  /**assigning values to elements of struct
   * must be done within a function
   * Otherwise there will be "does not name a type" error
   */
  P2.X=0.0;
  P2.Y=0.0;
  P2.F=0;
  Serial.begin(BAUD);
  help();
  sready();
  /**thinking of adding an init function that will move
	 the header to the origin position if it is already
	 not there...
	 it will be different from sready() in the sense that
	 the buffer will remain unaffected. sready() prepares
	 for receiving next command only... **/
  origin();
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

/**An init command apart from gcode defined ones
takes the head to origin of the coord system*/
void origin()
{
	if(P2.X!=0.0 || P2.Y!=0.0)
	{
		sX.setSpeed(P1.F);
		sY.setSpeed(P1.F);
		sX.step(-P1.X);
		sY.step(-P1.Y);
	}
}

/**
 * display helpful information
 */
void help() 
{
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

//process the GCODE command that is thrown
void processCommand(String str)
{
  //Looking for commands that start with G:
  if(str.charAt(0)=='G')
  {
    int c1=str.substring(1,3).toInt();
    int P,d; //for pause since we cannot declare variables in switch cases
    switch(c1)
    {
      case 00: //Serial.println(P1.X);
               //Serial.println(P1.Y);
               //Serial.println(P1.F);
			         //get some function to draw/move the header
			         break;

      case 01: in1.cx=str.indexOf('X');
               in1.cy=str.indexOf('Y');
               in1.cf=str.indexOf('F');
               P1.X=str.substring(in1.cx+1, in1.cy).toFloat();
               P1.Y=str.substring(in1.cy+1, in1.cf).toFloat();
               P1.F=str.substring(in1.cf+1).toInt();
			         move();
               origin();
			         //gets the coords into P2 for reference in next steps
			         //P2=P1;
               break;

      case 02: //clockwise arc
               break;

      case 03: //anti-clockwise arc
               break;

      case 04: Serial.println("Wait");
               P=str.indexOf('P');
               d = str.substring(P+1).toInt();
               Serial.print("P at... ");
               Serial.println(P);
               Serial.print("waiting for ");
               Serial.println(d);
               delay(d);
               break;

      case 90: Serial.print("abs\n");
               break;
               
      case 91: Serial.print("rel\n");
               break;

      case 92: //set position
               break;
    }
  }
  
  if(str.charAt(0)=='M')
  {
    int c1=str.substring(1,4).toInt();
    switch(c1)
    {
      case 18:  Serial.println("disable motors");
                break;

      case 100: help();
                break;

      case 114: Serial.println("I am at: ");
                Serial.print(P1.X);
                Serial.print(',');
                Serial.println(P1.Y);
                Serial.print("Feedrate: ");
                Serial.println(P1.F);
                break;

      case 300: Serial.println("Pen UP/DOWN");
                int cs = str.indexOf('S');
                float val = str.substring(cs+1).toFloat();
                if(val==50.0)
                {
                  Serial.println(val);
                  penUp();
                }
                else if(val==30.0)
                {
                  Serial.println(val);
                  penDown();                
                }
    }
  }
}

//Function to draw lines
void move()
{
	//Do something awesome
	float mx = P1.X-P2.X;
	float my = P1.Y-P2.Y;
  Serial.println("moving...");
	sX.setSpeed(P1.F);
	sY.setSpeed(P1.F);
	sX.step(mx);
	sY.step(my);
}

void penUp()
{
  Serial.println("Pen Up");
}

void penDown()
{
  Serial.println("Pen Down");
}
