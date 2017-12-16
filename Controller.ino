#include <CommandHandler.h>
#include <Menu.h>
#include <Source.h>

CommandHandler ch;

void setup()
{
    ch.boot();
}

void loop()
{
    ch.processCommand();
}
