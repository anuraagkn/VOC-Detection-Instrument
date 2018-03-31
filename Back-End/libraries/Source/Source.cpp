/*Source.cpp by Anuraag Narang.

A collection of functions to operate the input signals of the detection device.
Appropriate functions are called from the Menu class when a :SOURce command recieved.

*/

//#include "Arduino.h"
#include "Source.h"
//#include <math.h> // log() function for sweep

MCP6S21 PGA(10);    // *need to change the pin value*

Source::Source()
{

}

//======================== PUMP METHODS ==========================

// set air flow into LC cell
void Source::setPumpFlow(String v)
{
    vol = v.toInt();
    switch(vol)
    {
        case 1: vol = 250; break;
        case 2: vol = 200; break;
        case 3: vol = 150; break;
        case 4: vol = 100; break;
        case 5: vol = 50; break;
        default: vol = 150; break;
    }

    // CONVERT TO DUTY CYCLE VALUE (volDuty?)

    Serial.println("flow=" + String(vol));

}

// set pump time
void Source::setPumpTime(String t)
{
    time = t.toFloat() * 1000; // convert seconds to milliseconds
    Serial.println("pumptime=" + String(time) + "ms");
}

// pump air for the defined time at the defined flow level
void Source::pump(String p, float t)
{
    // MAYBE DO THE CONVERSION HERE?

   if (p == "ON")
   {
        Serial.println(F("pumpon"));
        //analogWrite(PWM, volDuty);
        pinMode(9,OUTPUT);
        analogWrite(9, vol);
        delay(t); // pump for specified time
        analogWrite(9, 0); // stop pumping
        Serial.println(F("pumpoff"));
    }
    else
    {
        Serial.println("Error: Invalid argument " + p + ". Expected \"ON\".");
    }
}

//================================================================

void Source::AD9833Reset()
{
	AD9833Write(0x100);  // Write '1' to AD9833 Control register bit D8.
	delay(10);
}

void Source::AD9833Write(int data)
{
	digitalWrite(FSYNC, LOW);     // Set FSYNC low before writing to AD9833 registers
	delayMicroseconds(10);        // Give AD9833 time to get ready to receive data.
	SPI.transfer(highByte(data)); // Each AD9833 register is 32 bits wide and each 16
	SPI.transfer(lowByte(data));  // bits has to be transferred as 2 x 8-bit bytes.
	digitalWrite(FSYNC, HIGH);    // Write done. Set FSYNC high
}

// ================================================================================
// ========================== Function Generator Methods ==========================
// ================================================================================

void Source::getFreq(String s)
{
    if (s == "STARt"){Serial.println(freq);}
}

// set input waveform frequency
void Source::setFreq(String f)
{
	if (f.toFloat() < 0.0 || f.toFloat() > 12500000.0)	// check for valid frequency
	{
		Serial.println("Error: Frequency"  + String(f.toFloat()) + "out of range. Range is 0Hz - 12.5MHz");
	}
	else // frequency is valid so assign it
	{
		freq = f.toFloat();			// convert to float and set
		long freqWord = (freq * pow(2, 28)) / CLK_AD9833;
		// set the frequency value on the 9833
		int MSB = (int)((freqWord & 0xFFFC000) >> 14);    //Only lower 14 bits are used for data
		int LSB = (int)(freqWord & 0x3FFF);
		//Set control bits 15 ande 14 to 0 and 1, respectively, for frequency register 0
		LSB |= 0x4000;
		MSB |= 0x4000;
		// write data
		AD9833Write(0x2100);
		AD9833Write(LSB);     // Write lower 16 bits to AD9833 registers
		AD9833Write(MSB);     // Write upper 16 bits to AD9833 registers.
		AD9833Write(0xC000);  // Phase register
		SPI.setDataMode(SPI_MODE2); // set AD9833 SPI mode
		AD9833Write(wave);    // Exit & Reset to wave

		Serial.println("freq=" + String(freq));
	}
}

// set waveform amplitude by adjusting the potentiometer step
void Source::setAmp(String a)
{
	ampl = a.toInt(); // set amplitude variable. This can be returned in a query
    switch(ampl)
    {
        case 1: PGA.gain(x1); displayValue = 3.84; break;
        case 2: PGA.gain(x2); displayValue = 7.04; break;
        case 3: PGA.gain(x4); displayValue = 14.0; break;
        case 4: PGA.gain(x5); displayValue = 17.8; break;
        case 5: PGA.gain(x8); displayValue = 28.4; break;
        default: Serial.println(F("Error: Invalid amplitude level. Valid range is 1-5")); break;
    }
    Serial.println("v=" + String(displayValue));// + " ampl = " + String(ampl));
}

// set input waveform wavetype
void Source::setWave(String w)
{
	if (w == "SINE" || w == "SQUAre" || w == "TRIangular") {

		if (w == "SINE") { wave = SINE; }
		else if (w == "SQUAre") { wave = SQUARE; }
		else if (w == "TRIangular") { wave = TRIANGULAR; }

		SPI.setDataMode(SPI_MODE2); // set AD9833 SPI mode
		AD9833Write(wave);        // tell the 9833 the register value
		Serial.println("type=" + w);
	}
	else { Serial.println(F("Error: Invalid waveform. Please enter a command with a valid waveform choice (SINE?, SQUARE?, TRIANGLE?).")); }
}
/*
// sweep the defined frequency range
// called when user initiates frequency sweep
void Source::sweep(String b)
{
	if (b == "BEGIn")
	{
		delay(1000);                // WHAT'S THE POINT OF THIS?
		int x = 0;                  // sweep point counter
		float fstart = freq;        // set start frequency (cant use freq - it will be modified during the loop)
		float span = fstop - freq;  // frequency span of the sweep
		float fsweep = freq;        // initialise sweep frequency
		//Serial.println("Sweep freq initialised to " + String(freq));

        // sweep through the frequency range(span)
		while (x <= (points - 1))
        {

            // pump per frequency point if user desires
            if (pppoint == "Y")
            {
                setFreq(String("STARt"), String(0)); // set freq = 0
                // and stop recording results
                pump("ON"); // pump air based on user-defined parameters

                // after pump finished, frequency will be set to fsweep in the code below.
                // so no need to restore it here.
            }

            // compute frequency increment based on scale
            if (scale == "LIN"){ fsweep = fstart + x * (span / (points - 1)); }// fstart + x*(span/npoints)
            else if (scale == "LOG"){ fsweep = fstart + span * log(x+1)/log(points); }

            setFreq(String("STARt"), String(fsweep));   // set freq to fsweep

            // start recording results

            delay(hold);                                // hold frequency for defined time period

            // stop recording results

            x++;                                        // increment sweep point count
        }
        // reset freq to initial when finished sweep
        setFreq(String("STARt"), String(fstart));
	}
	else{ Serial.println("Error: invalid command " + b); } // if arg != BEGIn
}
*/
// write data to the selected digital potentiometer
void Source::potWrite(int cs, int data)
{
	digitalWrite(cs, LOW);
	SPI.transfer(ADDR);
	SPI.transfer(data);
	digitalWrite(cs, HIGH);
}

// set the brightness level of the LED
void Source::setLamp(String l)
{
    lamp = l.toInt();
    if (lamp < 0 || lamp > 200)
    {
		Serial.println("Error: Lamp brightness "  + String(lamp) + " is out of range. Range is 0 - 200.");
	}
	else
    {
        potWrite(CS_LED, lamp); // write value to pot
        Serial.println("lamp=" + String(lamp));
   }
}
