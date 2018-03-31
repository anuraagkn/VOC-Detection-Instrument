
#include <SPI.h>
#include <math.h> // log() function for sweep
#include "Arduino.h"
#include <MCP6S21.h> // gain controlled amp lib

// ad9833 clock frequency
#define CLK_AD9833  25000000.0

// ad9833 pins
#define FSYNC       10
#define CLK         13
#define DATA        11

//#define FSYNC       2
//#define CLK         3
//#define DATA        4

// digital pot chip select pins
//NEED TO CHANGE THESE
//#define CS_AMP		10
#define CS_AMP		5
#define CS_LED		10
//#define PWM         9

#define ADDR		(byte)0x00

// wavetype registers
#define SINE        (int)0x2000
#define SQUARE      (int)0x2028
#define TRIANGULAR   (int)0x2002

class Source
{
	public:

		Source(); // constructor

		// pump methods

		void setPumpFlow(String); // set volume of air flowing per time unit
		void setPumpTime(String); // set time for running pump
		void pump(String, float); // pump air using defined parameters

		// function generator methods

		void AD9833Reset();
		void AD9833Write(int data);

        void getFreq(String);   // get frequency
		void setFreq(String);	// set frequency

		void setAmp(String a);	// set amplitude
		void setWave(String w);	// set wave type

		void sweep(String);			// initiate sweep

		void potWrite(int, int);	// write data to digital pot

		// lamp methods

		void setLamp(String); // LED brightness

		float freq; // waveform frequency & start freq for sweep
		float time; // time for pumping

	private:

        //MCP6S21 PGA(10);    // *need to change the pin value*

		// pump parameters
		int vol;	// volume of flow per time unit
		int PWM = 9;

		// waveform parameters
		int ampl; // waveform amplitude
		float displayValue = 3.84; // default
		int   wave; // waveform type

        int lamp;   // LED brightness
};
