#include "Arduino.h"
#include "Source.h"
#include <math.h> // log() function for sweep
//#include "Menu.h"

Source::Source()
{

}

//======================== PUMP METHODS ==========================

void Source::setPumpFlow(String vol)
{

}

void Source::setPumpTime(String vol)
{

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

// set input waveform frequency
void Source::setFreq(String s, String f)
{
	if (f.toFloat() < 0.0 || f.toFloat() > 12500000.0)	// check for valid frequency
	{
		Serial.println("Error: Frequency"  + String(f.toFloat()) + "out of range. Range is 0Hz - 12.5MHz");
	}

	// frequency is valid
	else
	{
		// assign frequency value
		if (s == "STARt")				// assign start frequency
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

			Serial.println("Start freq set to " + String(freq));
		}
		else if (s == "STOP")			// assign stop frequency
		{
			fstop = f.toFloat();		// convertto float and set
			Serial.println("Stop freq set to " + String(fstop));
		}
	}

	//String freqString = String(freq);
	//Serial.println("Frequency set to " + freqString + "Hz"); // test
}

// set waveform amplitude by adjusting the potentiometer step
void Source::setAmp(String a)
{
	int n;

	//n = ((rTwo / ((a.toFloat() / vDDS) - 1)) / rMax) * N;
	//ampl = a.toFloat();	// set amplitude variable
						// this can be returned in a query
	// test statement
	//Serial.println("Amplitude set to " + String(ampl) + "V");

	// ============= NEW METHOD =============
	ampl = a.toFloat(); // set amplitude variable
                    // this can be returned in a query
	Serial.println("Amplitude set to " + String(ampl) + "V");

    // pin = v/vmax * 128
    //n = round(128-((a.toFloat()/vMax) * 128));
    //potWrite(CS_AMP, n ); // need to determine cs
    potWrite(CS_AMP, round(a.toFloat()) ); // need to determine cs
    Serial.println("Pot pin set to " + String(a));

	/* OLD METHOD


	//0 - 128 steps
	// max practival value = 70
	// above 70: gain too small, signal noisy

	// this method works theoretically,
	// in practice, all the steps of the pot
	// will not be usable due to the distortion (low R)
	// and clipping (high R) by the amplifier

	// check for valid value
	if (a.toFloat() < 9.8 || a.toFloat() > 30)
	{
		Serial.println(F("Error: Amplitude must be in the range between 9.8V and 30V"));
	}
	else
	{
		// compute pin number corresponding to amplitude a
		int pin = (a.toFloat() - 38.08) / (-0.404);

		potWrite(CS_AMP, pin);	// write pin number to amplifier pot

		ampl = a.toFloat();	// set amplitude variable
							// this can be returned in a query

							// test statement
		Serial.println("Amplitude set to " + String(ampl) + "V");
	}*/

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
		Serial.println("Wave type set to " + w);
	}
	else { Serial.println(F("Error: Invalid waveform. Please enter a command with a valid waveform choice (SINE?, SQUARE?, TRIANGLE?).")); }
}

// set scale for frequency sweep
void Source::setScale(String s)
{
	scale = s;
	Serial.println("Scale set to " + scale);
}

// set number of frequency points in the sweep
void Source::setPoints(String n)
{
	points = n.toFloat();
	Serial.println("Interval set to " + String(points));
}

// set the time to wait at each frequency point in the sweep
void Source::setHold(String h)
{
	hold = 1000 * h.toFloat();	// convert to milliseconds
	Serial.println("Hold set to " + String(hold));
}

// sweep the defined frequency range
// called when user initiates frequency sweep
//GUI: Don't write start freq until sweep called
void Source::sweep(String b)
{
	if (b == "BEGIn")
	{
		delay(1000);

		int x = 0; // sweep point counter

		float fstart = freq; // set start frequency (cant use freq;
                                               // it will be modified during the loop)

		float span = fstop - freq; // frequency span of the sweep

		// initialise sweep frequency
		float fsweep = freq;
		//Serial.println("Sweep freq initialised to " + String(freq));

		while (x <= (points - 1)) // sweep through the frequency range(span)
        {
            // if pump = yes
                // set freq = 0
                // turn pump on (with some speed)
                // hold for user defined time
                // turn pump off

            // compute frequency increment
            if (scale == "LIN")
            {
                fsweep = fstart + x * (span / (points - 1)); // fstart + x*(span/npoints)
            }
            else if (scale == "LOG")
            {
                fsweep = fstart + span * log(x+1)/log(points);
            }

            setFreq(String("STARt"), String(fsweep));// set freq to fsweep
            delay(hold); // hold frequency for defined time period
            x++; // increment sweep point count



        }
        // reset freq to initial when finished sweep
        setFreq(String("STARt"), String(fstart));

	}
	else
	{
		Serial.println("Error: invalid command " + b);
	}
}

// write data to the selected digital potentiometer
void Source::potWrite(int cs, int data)
{
	digitalWrite(cs, LOW);
	SPI.transfer(ADDR);
	SPI.transfer(data);
	digitalWrite(cs, HIGH);
}
