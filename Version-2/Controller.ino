#include <Controller.h>
#include <Menu.h>

Controller c;

void setup()
{
    c.boot();
}

void loop()
{
    c.processCommand();
}
