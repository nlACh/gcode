#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF 64
char buffer[MAX_BUF];
int sofar=0, i=0; //how much is in the buffer
char c;

int main()
{
    scanf("%c", &c); // get it
    while(1)
  {
	// if something is available

    //Serial.print(c); // optional: repeat back what I got for debugging

    // store the byte as long as there's room in the buffer.
    // if the buffer is full some data might get lost
    if(sofar < MAX_BUF)
		buffer[sofar++]=c;
	else
	{
		("Buffer Overflow!!");
		sready();
		break;
	}

	// if we got a return character (\n) the message is done.
	if((c=='\n')||(c==';'))
	{
      //Serial.print(F("\r\n")); // optional: send back a return for debugging

      // strings must end with a \0.
      buffer[sofar]='\0';
      processCommand(); // do something with the command
      sready();
    }
  }
}

void sready()
{
    sofar=0;
    printf("Ready for new code");
}

void processCommand()
{
    printf("Entered code");
    printf(buffer);
}
