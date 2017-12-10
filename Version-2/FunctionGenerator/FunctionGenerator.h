/* Created using arduino.cc/en/Hacking/LibraryTutorial
*/

#ifndef FunctionGenerator_h
#define FunctionGenerator_h

#include "Arduino.h"

#include <SPI.h>

// ad9833 clock frequency
#define CLK_AD9833  25000000.0

// ad9833 pins
#define FSYNC       10
#define CLK         13
#define DATA        11

// wavetype registers
#define SINE        0x2000
#define SQUARE      0x2028
#define TRIANGLE    0x2002

class FunctionGenerator
{
	public:
		FunctionGenerator(float f, float a, int w); // constructor
		void AD9833Reset();
		void AD9833Write(int data);
		// function generator methods
		void setFreq(float f);	// set frequency
		String getFreq();
		void setAmp(String a);	// set amplitude
		String getAmp();
		void setWave(String w);	// set wave type
		void sweep(float sf, float fs, String s, float i, int t);
	private:
		float freq; // waveform frequency
		float ampl; // waveform amplitude
		int   wave; // waveform type

		// sweep parameters
		float stopFreq;
		String scale;
		float interval;
		int sweepTime;
};

#endif