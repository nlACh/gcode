/**
 * prepares the input buffer to receive a new message and 
 * tells the serial connected device it is ready for more.
 */
void sready(int val) 
{
  val=0; // clear input buffer
  Serial.print(F("> ")); // signal ready to receive input
}
