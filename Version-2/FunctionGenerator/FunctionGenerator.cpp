#include "Arduino.h"
#include "FunctionGenerator.h"
//#include "Menu.h"

// digi pot instance 1
// digi pot instance 2

FunctionGenerator::FunctionGenerator()
{

}

void FunctionGenerator::AD9833Reset()
{
	AD9833Write(0x100);  // Write '1' to AD9833 Control register bit D8.
	delay(10);
}

void FunctionGenerator::AD9833Write(int data)
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

//////////

// adapt this to set either stop or start freq

//////////
void FunctionGenerator::setFreq(String s, String f)// string end
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
		AD9833Write(wave);    // Exit & Reset to wave

		Serial.println("Start freq set to " + String(freq));
	}
	else if (s == "STOP")			// assign stop frequency
	{ 
		stopFreq = f.toFloat();		// convertto float and set
		Serial.println("Stop freq set to " + String(stopFreq));
	}

	//String freqString = String(freq);
	//Serial.println("Frequency set to " + freqString + "Hz"); // test
}

void FunctionGenerator::setAmp(String a)
{
	//0 - 128 steps
	// max practival value = 70
	// above 70: gain too small, signal noisy

	// this method works theoretically, 
	// in practice, all the steps of the pot 
	// will not be usable due to the distortion (low R)
	// and clipping (high R)


}

void FunctionGenerator::setWave(String w)
{
	if (w == "SINE") { wave = SINE; }
	else if (w == "SQUARE") { wave = SQUARE; }
	else if (w == "TRIANGLE") { wave = TRIANGLE; }

	if (w == "SINE" || w == "SQUARE" || w == "TRIANGLE") {
		SPI.setDataMode(SPI_MODE2); // set AD9833 SPI mode
		AD9833Write(wave);        // tell the 9833 the register value
		Serial.println("Wave type set to " + w);
	}
	else { Serial.println(F("Error: Invalid waveform. Please enter a command with a valid waveform choice (SINE?, SQUARE?, TRIANGLE?).")); }
}

void FunctionGenerator::setScale(String s)
{
	scale = s;
	Serial.println("Scale set to " + scale);
}

void FunctionGenerator::setInterval(String i)
{
	interval = i.toFloat();
	Serial.println("Interval set to " + String(interval));
}

void FunctionGenerator::setHold(String h)
{
	hold = 1000 * h.toFloat();	// convert to milliseconds
	Serial.println("Hold set to " + String(hold));
}

// sweep the defined frequency range
// called when user initiates frequency range
//GUI: Don't write start freq until sweep called
void FunctionGenerator::sweep(String b)
{
	if (b == "BEGIn")
	{
		// compute linear frequency increment
		float incFreq = (stopFreq - freq) / interval;
		Serial.println("Inc freq set to " + String(incFreq));

		// initialise sweep frequency
		float sweepFreq = freq;
		Serial.println("Sweep freq initialised to " + String(freq));

		// sweep the freqeuency range
		while (sweepFreq <= stopFreq)	// stop when sweep frequency passes max
		{
			setFreq(String("STARt"), String(sweepFreq));	// set frequency to sweep frequency
			delay(hold);		// hold the frequency for the defined time

			// increment sweep frequency based on the scale
			if (scale == "LIN") { sweepFreq = sweepFreq + incFreq; }
		}
	}
	else
	{
		Serial.println("Error: invalid command " + b);
	}
}

/*
If pump flag high:
	turn off func gen
	pump for predefined time
	inc freq and turn on func gen
*/