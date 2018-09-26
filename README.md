==================================================================================================

COMMAND							||	MEANING														||

==================================================================================================
G00 [X(number)][Y(number)][F]	||	Absolute mode: Move in a line to (x,y) at speed F			||
								||																||
G01 [X(number)][Y(number)][F]	||	Relative mode: Move (x,y) amount at speed F					||
								||																||
G04 P(number)					||	Do nothing for P seconds									||
								||																||
G90								||	absolute mode												||
								||																||
G91								||	relative mode												||
								||																||
G92 [X(number)][Y(number)]		||	change logical position										||
								||																||
M18								||	turn off power to motors									||
								||																||
M100							||	print out instructions for the human						||
								||																||
M114							||	report position and feedrate								||

==================================================================================================

>>Every (number) is assumed to be a float – a number that might have a decimal place and an exponent. 2.015e-5 is a float.

>>Anything in [brackets] is optional.

>>G and M commands cannot be combined in a single instruction.

>>Arduino software has a nice way to send messages to the PCB through the serial interface window. It’s the magnifying glass on the right hand side of the Arduino   window. Unfortunately the serial interface window doesn’t send the return key (\n) to the PCB. Instead of return I’m going to use semicolon (;) to tell the machine   “this is the end of an instruction”.
