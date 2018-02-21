
#include <SPI.h>

// ad9833 clock frequency
#define CLK_AD9833  25000000.0

// ad9833 pins
#define FSYNC       10
//#define FSYNC       7
#define CLK         13
#define DATA        11

// digital pot chip select pins
//NEED TO CHANGE THESE
#define CS_AMP		10
#define CS_LED		10

#define ADDR		(int)0x00

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
		void pump(String); // pump air using defined parameters

		// function generator methods

		void AD9833Reset();
		void AD9833Write(int data);

		void setFreq(String, String);	// set frequency
		//String getFreq(String&); // get frequency

		void setAmp(String a);	// set amplitude
		//String getAmp();		// get amplitude

		void setWave(String w);	// set wave type
		//String getWave();		// get wave type

		void setScale(String);
		//String getScale();

		void setPoints(String);
		//String getInterval();

		void setHold(String);
		//String getHold();

		void sweep(String);			// initiate sweep
		//void linSweepInc();

		void potWrite(int, int);	// write data to digital pot

		// lamp methods

		void setBrightness(String); // LED brightness

	private:

		// pump parameters
		float vol;	// volume of flow per time unit
		float time; // time for pumping

		// waveform parameters
		float freq; // waveform frequency & start freq for sweep
		float ampl; // waveform amplitude
		int   wave; // waveform type

		// sweep parameters
		float fstop;	// stop frequnecy
		String scale;	// linear or log
		float points;	// number of frequencies
		int hold;	// time spent at each frequency
		String pppoint; // pump at each frequency point?

        float vMax = 30.0;  // max voltage out
		float vDDS = 0.65;		// max AD9833 output
		float rTwo = 47000.0;	// R2 in the voltage amp design
		float rMax = 10000.0;	// maximum resistance of the digi pot
		float N = 127.00;		// max step number on the pot (0 - 127)
};
