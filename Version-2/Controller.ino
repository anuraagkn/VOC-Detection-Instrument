#include <Controller.h>
#include <FunctionGenerator.h>

//FunctionGenerator fg(0.0,0.0,0);
Controller c(false);

void setup()
{
    c.bootInstrument();
}

void loop()
{
    c.operate();
}
