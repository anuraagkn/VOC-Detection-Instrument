/* Created using arduino.cc/en/Hacking/LibraryTutorial
*/

#ifndef FunctionGenerator_h
#define FunctionGenerator_h

#include "Arduino.h"

#include <SPI.h>

// ad9833 clock frequency
#define CLK_AD9833  25000000.0

// ad9833 pins
//#define FSYNC       10
//#define CLK         13
//#define DATA        11

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

		FunctionGenerator(); // constructor

		void AD9833Reset();
		void AD9833Write(int data);

		// function generator methods
		void setFreq(String, String);	// set frequency
		String getFreq(String&); // get frequency

		void setAmp(String a);	// set amplitude
		String getAmp();		// get amplitude
		
		void setWave(String w);	// set wave type
		String getWave();		// get wave type

		void setScale(String);
		String getScale();

		void setInterval(String);
		String getInterval();

		void setHold(String);
		String getHold();

		void sweep(String);			// initiate sweep

	private:

		// waveform parameters
		float freq; // waveform frequency & start freq for sweep
		float ampl; // waveform amplitude
		int   wave; // waveform type

		// sweep parameters
		float stopFreq;	// stop frequnecy
		String scale;	// linear or log
		float interval;	// number of frequencies
		int hold;	// time spent at each frequency
};

#endif