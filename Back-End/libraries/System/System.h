#include "Arduino.h"

class System
{
public:

	System(); // constructor
	void reset();
	void enact(int, String); // selects which method to call
	void runAutoExperiment(String);
	void setupAutoExperiment(char, String);

private:
	// auto exp parameters
	float startFreq;
	float stopFreq;
	float span;
	String scale;
	int points;
	float hold;
	String ppp;
};