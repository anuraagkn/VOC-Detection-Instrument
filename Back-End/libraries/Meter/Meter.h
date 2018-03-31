#include "Arduino.h"

class Meter
{
public:
	Meter();
	void reset(float); // reset measurement parameters
	void record(); // record data
private:
	// declaring variables
	int inputPin, del, increment, settle, samples,
		z, count, trig, data, markerdiff, T, measureFor;
	int marker[30], average[350];
	byte signalin[900], goodwaves[30];
	double waves, frequency;
};