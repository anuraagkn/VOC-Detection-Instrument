#include "Arduino.h"

// root nodes
#define SYST		"SYSTem"
#define SOURCE		"SOURce"

// nodes
	//source/
#define	FREQ		"FREQuency"
#define WAVE		"WAVEtype"

// leaf nodes
//syst/
#define RESET		"RESEt"
//source/frequency/
#define START		"STARt"
#define STOP		"STOP"
#define SCALE		"SCALe"
#define STEP		"STEP"
#define INTERVAL	"INTErval"
//source/voltage/
#define VOLT		"VOLTage"
//source/wavetype/
#define SINE		"SINE"
#define SQUARE		"SQUAre"
#define TRIANGLE	"TRIAngle"

class Menu
{
public:
	Menu();	// constructor
	void setRoot(String& r);
	// source menu
	// pump menu
	// output menu
private:
	String root;
	bool sys;
	bool src;

};