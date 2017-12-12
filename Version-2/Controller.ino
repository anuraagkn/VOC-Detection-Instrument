#include <Controller.h>
#include <Menu.h>
#include <FunctionGenerator.h>

Controller c;

void setup()
{
    c.boot();
}

void loop()
{
    c.processCommand();
}
