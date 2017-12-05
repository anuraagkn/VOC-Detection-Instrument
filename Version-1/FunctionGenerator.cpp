#include "Arduino.h"
#include "FunctionGenerator.h"

FunctionGenerator::FunctionGenerator(float f, float a, int w)
{
	freq = f;	// Hz
	ampl = a;	// V
	wave = w;
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

void FunctionGenerator::setFreq(float f)
{
	freq = f; // assign frequency value

			  // set the frequency value on the 9833

	long freqWord = (freq * pow(2, 28)) / CLK_AD9833;

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

	String freqString = String(freq);
	Serial.println("Frequency set to " + freqString + "Hz"); // test
}

void FunctionGenerator::setAmp(String a)
{
	
}

void FunctionGenerator::setWave(String w)
{
	if (w == "SINE?") { wave = SINE; }
	else if (w == "SQUARE?") { wave = SQUARE; }
	else if (w == "TRIANGLE?") { wave = TRIANGLE; }

	if (w == "SINE?" || w == "SQUARE?" || w == "TRIANGLE?") {
		SPI.setDataMode(SPI_MODE2); // set AD9833 SPI mode
		AD9833Write(wave);        // tell the 9833 the register value
	}
	else { Serial.println(F("Error: Invalid waveform. Please enter a command with a valid waveform choice (SINE?, SQUARE?, TRIANGLE?).")); }
}

void FunctionGenerator::sweep(float sf, float fs, String s, float i, int t)
{
	float startFreq = sf; // set start frequency
	Serial.println("Start freq set"); // test

	float stopFreq = fs;  // set stop frequency
	Serial.println("stop freq set"); // test

	String scale = s;                // set scale
	Serial.println("scale set to " + s); // test

	float interval = i;  // set interval
	Serial.println("interval set to " + String(interval)); // test

	int sweepTime = 1000 * t;     // set time per frequency in microseconds
	Serial.println("time set to" + String(sweepTime)); // test

	float incFreq = (stopFreq - startFreq) / interval; // set increment
	Serial.println("inc freq set to " + String(incFreq)); // test

	float sweepFreq = startFreq;              // initialise sweep frequency
	Serial.println("sweep freq initialised to " + String(sweepFreq)); // test

																	  // sweep the freqeuency range
	while (sweepFreq <= stopFreq) {
		//Serial.println(F("reached here"));
		setFreq(sweepFreq);
		delay(sweepTime);
		//Serial.println(F("reached here"));

		// increment frequency by interval
		if (scale == "LIN") {
			sweepFreq = sweepFreq + incFreq;
			//Serial.println(F("sweep freq incremented)");
		}
		//Serial.println(F("reached here"));
	}
}
