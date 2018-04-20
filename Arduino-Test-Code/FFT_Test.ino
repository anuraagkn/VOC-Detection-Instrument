#include "arduinoFFT.h"

arduinoFFT FFT = arduinoFFT();

uint16_t testSamples; //This value MUST ALWAYS be a power of 2
double testSignalFrequency;
double testSamplingFrequency;
uint8_t amplitude;

#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

// analyse()
void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);

  testSamples = 64;
  testSignalFrequency = 1000;
  testSamplingFrequency = 10000;
  amplitude = 100;

  double vReal[testSamples];
  double vImag[testSamples];

  generateData(vReal, vImag, testSamples, testSignalFrequency, testSamplingFrequency, amplitude);
  fastFourierTransform(vReal, vImag, testSamples, testSamplingFrequency);
  
}

void generateData(double *vR, double *vI, uint8_t sampl, double sigFreq, double samplFreq, uint8_t ampl)
{
  /* Build raw data */
  double cycles = (((sampl-1) * sigFreq) / samplFreq); //Number of signal cycles that the sampling will read
  for (uint16_t i = 0; i < sampl; i++)
  {
    vR[i] = int8_t((ampl * (sin((i * (twoPi * cycles)) / sampl))) / 2.0);/* Build data with positive and negative values*/
    vI[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }
}

void fastFourierTransform(double *vR, double *vI, uint8_t sampl, double samplFreq)
{
  /* Print the results of the simulated sampling according to time */
  Serial.println("Data:");
  printVector(vR, sampl, SCL_TIME, sampl, samplFreq);
  FFT.Windowing(vR, sampl, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  Serial.println("Weighed data:");
  printVector(vR, sampl, SCL_TIME, sampl, samplFreq);
  FFT.Compute(vR, vI, sampl, FFT_FORWARD); /* Compute FFT */
  Serial.println("Computed Real values:");
  printVector(vR, sampl, SCL_INDEX, sampl, samplFreq);
  Serial.println("Computed Imaginary values:");
  printVector(vI, sampl, SCL_INDEX, sampl, samplFreq);
  FFT.ComplexToMagnitude(vR, vI, sampl); /* Compute magnitudes */
  Serial.println("Computed magnitudes:");
  printVector(vR, (sampl >> 1), SCL_FREQUENCY, sampl, samplFreq);
  double x = FFT.MajorPeak(vR, sampl, samplFreq);
  Serial.println(x, 6);
}


void printVector(double *vData, uint16_t bufferSize, uint8_t scaleType, uint8_t sampl, double samplFreq)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    double abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplFreq);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplFreq) / sampl);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
