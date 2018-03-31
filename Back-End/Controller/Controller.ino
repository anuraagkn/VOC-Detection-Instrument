/*
Usage notes:
- Although input doubles are rounded to two decimal places, an abnormaly large number 
of decimal places may causes the system to fail.
*/

// instrument libraries
#include <CommandHandler.h>

CommandHandler ch;  // instantiate command handler object

// this code runs once when starting the program
void setup()
{
    ch.boot();  // set the defaults for the instrument
}

// this code runs repeatedly
void loop() 
{
    ch.processCommand();  // check for and process a command
}

