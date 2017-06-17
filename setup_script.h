/**use this piece code with a proper arduino ino code
since this code uses Serial.* functions that are only provided
by the arduino code.
**/

#define BAUD (57600) //define speed of communication
#define MAX_BUF (64) //max length of message arduino can store
#define VERSION ('0.1c')
char buffer[MAX_BUF];
int sofar; //how much is in the buffer

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
