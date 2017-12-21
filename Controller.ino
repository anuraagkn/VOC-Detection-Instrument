// instrument libraries
#include <CommandHandler.h>
#include <Menu.h>
#include <Source.h>

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
