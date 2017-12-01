#include <Controller.h>
#include <FunctionGenerator.h>

Controller c(false);

void setup()
{
    c.bootInstrument();
}

void loop()
{
    c.operate();
}
