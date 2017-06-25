#include <AccelStepper.h>
//search for modified code with ACCEL::
//Switching to AccelStepper library
//hoping to control 2 motors simultaneously...
#include <MultiStepper.h>//this baby can handle upto 10 motors
#include <Servo.h>
#include <String.h>

//version number
#define VERSION 0.65

//steps per revolution SPR of motors used
//change this when changing motors
#define SPR 64

#define BAUD 115200 //define speed of communication
#define MAX_BUF 64 //max length of message arduino can store

char buffer[MAX_BUF];
long sofar=0, i=0; //how much is in the buffer
//a check to disable motors
bool disabled=false;
//ACCEL::
AccelStepper sX(AccelStepper::FULL4WIRE, 2, 4, 3, 5); //X axis is the one woefully lying on the ground apparently
AccelStepper sY(AccelStepper::FULL4WIRE, 8, 10, 9, 11); //Y axis is the one hnging on the beams

MultiStepper s;
/**
 * Changing the params of the struct can allow operation in Z axis too
 * We will need z position when using another motor with controller.
 * Having a Z axis is good for 3D printers and CNC
 * Higher number of motors can be used for CNCs with more axes of movement (apparently they are expensive)
 * Plus an arduino with more I/O pins. It would be awesome to address the motors somehow by I2C
 */
struct point
{
  long X;
  long Y;
  long F;
};

struct index
{
	long cx;
	long cy;
	long cf;
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
  //configure each stepper
  //ACCEL::
  sX.setMaxSpeed(100);
  //sX.setAcceleration(60);
  sY.setMaxSpeed(100);
  //sY.setAcceleration(60);
  //ACCEL::
  //Give the steppers to multiStepper to manage
  s.addStepper(sX);
  s.addStepper(sY);
  //more later
  help();
  sready();
  /**
   * An absolute move function to move header to
   * desired coords, including origin
   * origin();
   */
  //move(0.0,0.0);
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

/**
 * An init command apart from gcode defined ones
 * takes the head to origin of the coord system
 * void origin()
 * {
 *     Serial.print(P2.X);Serial.println(P2.Y);
 *     sX.setSpeed(P2.F);
 *     sY.setSpeed(P2.F);
 *     sX.step(-P2.X);
 *     sY.step(-P2.Y);
 * 
 *  } 
 *This has been superceeded by the move(x,y) function
 *which can also be executed by 'G92X[]Y[]' GCODE
 *Note that there has been use of polymorphism in case of
 *this move() function.(See way down the code to understand...)
 */

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
  Serial.println(F("M300; - pen Up/Down control S50 for up and S30 for down"));
}

//process the GCODE command that is thrown
void processCommand(String str)
{
  //Looking for commands that start with G:
  if(str.charAt(0)=='G' || str.charAt(0)=='g')
  {
    long c1=str.substring(1,3).toInt();
    //long P,d; //for pause since we cannot declare variables in switch cases
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
               P1.X=str.substring(in1.cx+1, in1.cy).toInt();
               P1.Y=str.substring(in1.cy+1, in1.cf).toInt();
               P1.F=str.substring(in1.cf+1).toInt();
			         move();
			         //gets the coords into P2 for reference in next steps
			         P2=P1;
               break;

      case 02: //clockwise arc
               break;

      case 03: //anti-clockwise arc
               break;

      case 04: long P,d;
               Serial.println("Wait");
               P=str.indexOf('P');
               d = str.substring(P+1).toInt();
               Serial.print("P at... ");
               Serial.println(P);
               Serial.print("waiting for ");
               delay(d);
               Serial.println(d);
               break;

      case 90: Serial.print("abs\n");
               break;
               
      case 91: Serial.print("rel\n");
               break;

      case 92: in1.cx=str.indexOf('X');
               in1.cy=str.indexOf('Y');
               P1.X=str.substring(in1.cx+1, in1.cy).toInt();
               P1.Y=str.substring(in1.cy+1, in1.cf).toInt();
               move(P1.X,P1.Y);
               break;
    }
  }
  
  if(str.charAt(0)=='M' || str.charAt(0)=='m')
  {
    long c1=str.substring(1,4).toInt();
    switch(c1)
    {
      case 18:  Serial.println("disabling motors...");
                for(long i=2; i<=5; i++)
                  digitalWrite(i,LOW);
                for(long j=8; j<=11; j++)
                  digitalWrite(j,LOW);
                disabled=true;
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
                long cs = str.indexOf('S');
                long val = str.substring(cs+1).toInt();
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
  //rationalize(); new function to make coords into steps
  //based on scaling factors on both axes...
  long pos[2];
	//Doing something awesome
	pos[0] = P1.X-P2.X;
	pos[1] = P1.Y-P2.Y;
  Serial.println("moving...");
  //ACCEL::
  s.moveTo(pos);
  s.runSpeedToPosition();
  //Blocks are in position to prevent any hijack
  //of important control.
}

//absolute move to coords
void move(long x, long y)
{
  //rationalize(); same condition as above...
  long pos[2];
  pos[0] = x-P2.X;
  pos[1] = y-P2.Y;
  //ACCEL::
  s.moveTo(pos);
  s.runSpeedToPosition();
  //Blocks are in position to prevent any hijack
  //of important control.
}

void penUp()
{
  Serial.println("Pen Up");
  //coming later when I get a servo
}

void penDown()
{
  Serial.println("Pen Down");
  //read the above comment
}
