#include "System.h"
#include "Source.h"
#include "Meter.h"

// instantiate source and meter objects
Source src;
Meter mtr;

// constructor
System::System()
{

}

// set all instrument parameters to default values
void System::reset()
{
	src.setPumpFlow(String("1"));
	src.setPumpTime(String("2.0"));
	src.AD9833Reset(); // reset dds chip
	// function generator defaults
	src.setFreq(String(1000.0));
	src.setAmp(String(3));
	src.setWave(String("TRIangular"));
	src.setLamp(String(100));
	// default auto exp values
	setupAutoExperiment('1', String(1000.0));
	setupAutoExperiment('2', String(10000.0));
	setupAutoExperiment('3', String("LIN"));
	setupAutoExperiment('4', String(9));
	setupAutoExperiment('5', String(2.0));
	setupAutoExperiment('6', String("Y"));
	mtr.reset(src.freq); // reset meter
}

// enact a command according to the action id
void System::enact(int actionid, String val)
{
	switch (actionid)
	{
	// system actions
	case 0: Serial.println(F("No action required")); break;
	case 1: reset(); break;
	case 2: setupAutoExperiment('1', val); break; // start freq
	case 3: setupAutoExperiment('2', val); break; // stop freq
	case 4: setupAutoExperiment('3', val); break; // freq scale
	case 5: setupAutoExperiment('4', val); break; // # of freq points
	case 6: setupAutoExperiment('5', val); break; // hold
	case 7: setupAutoExperiment('6', val); break; // pump per point?
	case 8: runAutoExperiment(val); break;
	// source actions
	case 9: src.setFreq(val); break;
	case 10: src.setAmp(val); break;
	case 11: src.setWave(val); break;
	case 12: src.setLamp(val); break;
	case 13: src.setPumpFlow(val); break;
	case 14: src.setPumpTime(val); break;
	case 15: src.pump(val, src.time); break;
	// meter actions
	default: Serial.println(F("Error: Invalid action id")); break;
	}
}

// assign value to specified auto exp parameter
void System::setupAutoExperiment(char paramid, String val)
{
	switch (paramid) // check the param id and assign the value accordingly
	{
	// start freq
	case '1':	if (val.toFloat() >= 0.0 && val.toFloat() <= 12500000.0) 
				{ 
					startFreq = val.toFloat(); span = stopFreq - startFreq;
				}
				else { Serial.println(F("Out of range.")); }
				break;
	// stop freq
	case '2':	if (val.toFloat() >= 0.0 && val.toFloat() <= 12500000.0)
				{
					stopFreq = val.toFloat(); span = stopFreq - startFreq;
				}
				else { Serial.println(F("Out of range.")); }
				break;
	// freq scale
	case '3':	if (val == "LIN" || val == "LOG")
				{
					scale = val;
				}
				else { Serial.println(F("Invalid scale.")); }
				break;
	// # of freq points
	case '4':	points = val.toInt(); break;
	// hold
	case '5':	hold = 1000 * val.toFloat(); break; // convert to ms
	// pump per point?
	case '6':	if (val == "Y" || val == "N")
				{
					ppp = val;
				}
				else { Serial.println("Invalid argument" + val); }
				break;
	default: Serial.println(F("Error: Invalid paramater id.")); break;
	}
	
}

// run automated experiment
void System::runAutoExperiment(String b)
{
	// check for valid argument then run auto experiment accordingly
	if (b != "BEGIn") { Serial.println("Error: Invalid argument " + b); }
	else
	{
		// auto experiment

		float freqPoints[19]; // max 20 frequency points

		// calculate frequency points
		for (int j = 0; j < points; j++)
		{
			scale == "LIN" ? freqPoints[j] = startFreq + j * (span / (points - 1)) // linear sweep
				: freqPoints[j] = startFreq + span * log(j + 1) / log(points);     // else log sweep
		}

		// run the experiment
		for (int j = 0; j < points; j++)
		{
			// pump if specified
			if (ppp == "Y")
			{
				src.setFreq(String("0")); // turn off waveform
				src.pump("ON", src.time); // pump analyte
				// waveform will be turned on later anyways
			}

			src.setFreq(String(freqPoints[j])); // update waveform frequency
			delay(hold); // allow LC cell to respond
			//mtr.sample;
		}

		src.setFreq(String(src.freq)); // reset frequency to pre-experiment value
	}
}