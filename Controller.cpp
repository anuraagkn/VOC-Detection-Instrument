#include "Arduino.h"
#include "Controller.h"
#include "FunctionGenerator.h"

FunctionGenerator fg(10000.0, 20.0, SINE); // 10kHz 20V sine wave

Controller::Controller(bool w)
{
	wait = w;
}

// called in Arduino setup
void Controller::bootInstrument()
{
	SPI.begin();
	fg.AD9833Reset();	// reset ad9833
	delay(50);
	
	Serial.begin(9600); 	// open serial port
	Serial.println(F("<INSTRUMENT IS READY>"));
}

// get the next operator in the SCPI command
void Controller::getNextOperator() 
{
	static byte ndx = 0;

	// define markers
	char startMarker	= ':';
	char stopMarker		= ';';
	char spaceMarker	= ' ';
	char openMarker		= '{';
	char closeMarker	= '}';

	char rc;

	// while data available in serial buffer && controller isn't waiting
	while (Serial.available() > 0 && wait == false) {
		rc = Serial.read();
		delayMicroseconds(1500);// next char may not be ready by time 
								// code is processed, so wait before processing

		// read the next character into the operator name
		if (rc != startMarker && rc != stopMarker && rc != spaceMarker && rc != openMarker && rc != closeMarker) {
			op[ndx] = rc;                         // add read char onto end of operator name
			ndx++;                                // increment index

			if (ndx >= OP_SIZE) { ndx = OP_SIZE - 1; } // handle overflow
		}

		// if the next character is one of the markers
		else {
			op[ndx] = '\0'; // terminate the string  
			ndx = 0;    // reset index
			wait = true; // controller pauses so operator can be processed
		}
	}
}

// implement the operator
void Controller::operate()
{
	if (wait == true) {

		String opVal = String(op);

		if (opVal == "WAVE") {         // set wave type

			wait = false; getNextOperator(); // get wave type
			String w = String(op);
			fg.setWave(w);

		}
		else if (opVal == "FREQ") {   // set frequency
			wait = false; getNextOperator(); // get frequency value
			float f = (String(op)).toFloat();
			fg.setFreq(f);
		}
		else if (opVal == "SWEEP")
		{   // initiate sweep
			Serial.println(F("Calling sweep"));

			wait = false; getNextOperator();
			float sf = (String(op)).toFloat();

			wait = false; getNextOperator();
			float fs = (String(op)).toFloat();

			wait = false; getNextOperator();
			String s = String(op);

			wait = false; getNextOperator();
			float i = (String(op)).toFloat();

			wait = false; getNextOperator();
			int t = (String(op)).toInt();

			fg.sweep(sf, fs, s, i, t);
		}                             // finished handling command

		wait = false;                 // let controller read next command

	}
	else { getNextOperator(); }
}
