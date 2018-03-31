// Measurement.cpp by Elliot Page and Anuraag Narang
// LC cell output is measured for the time specified (HOLD)

#include "Meter.h"
#include "arduinoFFT.h"

// constructor
Meter::Meter()
{

}

void Meter::reset(float f)
{
	inputPin = 3, del = 0, increment = 0, settle = 50, samples = 0,
		z = 0, count = 0, trig = 135, data = 0, T = 0;
	waves = 0, frequency = f;

	// set prescaler
	ADCSRA &= ~(bit(ADPS0) | bit(ADPS1) | bit(ADPS2)); // clear prescaler bits
	if (frequency >= 600) { ADCSRA |= bit(ADPS1); } // 4}
	else if (frequency >= 400) { ADCSRA |= bit(ADPS0) | bit(ADPS1); } // 8 
	else if (frequency >= 200) { ADCSRA |= bit(ADPS2); } // 16 
}

// record data
void Meter::record()
{
	// sample the signal
	double startTime = micros();

	for (int i = 0; i<900; i++)
	{
		signalin[i] = (analogRead(inputPin)) >> 2;
	}

	double endTime = micros();
	//***what do these parameters do?***
	waves = ((endTime - startTime) / 1000000) / (1 / (frequency));
	T = 900 / waves;
	settle = T - 10;

	//***what does this loop do?***
	for (int p = 0; p < 900; p++)
	{
		if (p != 0 && signalin[p - 1] < trig && signalin[p] >= trig
			&& p - del >= 0 && count <= 30)
		{
			count++;
			del = p + settle;
			marker[z] = p;
			z = count;
		}
	}

	//***what does this loop do?***
	for (int j = 0; j<z; j++)
	{
		markerdiff = marker[j + 1] - marker[j];
		if (markerdiff >= (T - 10) && markerdiff <= (T + 10))
		{
			goodwaves[j] = 1;
			increment++;
		}
		else
			goodwaves[j] = 0;

		if (goodwaves == 30)
			break;
	}

	// Averaging the signal: (next two loops)

	//***what does this loop do?***
	for (int i = 0; i < count; i++)
	{
		if (samples == 30)
		{
			break;
		}
		for (int j = 0; j < T; j++)
		{
			average[j] = average[j] + (signalin[marker[i] + j] * goodwaves[i]);
		}
		if (goodwaves[i] == 1)
			samples++;
	}

	//***what does this loop do?***
	if (samples == 30)
	{
		// print the averaged T samples
		for (int j = 0; j < T; j++)
		{
			average[j] = average[j] / 30;
			Serial.println(average[j]); // processed samples
		}
		samples = 0;
		memset(average, 0, sizeof(average));
	}

	// reset parameters (is this neccessary?)
	count = 0;
	z = 0;
	del = 0;
	increment = 0;
	memset(goodwaves, 0, sizeof(goodwaves));
	memset(marker, 0, sizeof(marker));
}
