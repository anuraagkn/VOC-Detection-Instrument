#include "Arduino.h"

// root nodes
#define SYSTEM		"SYSTem"
#define SOURCE		"SOURce"

// nodes
	//source/
#define	FREQUENCY	"FREQuency"
#define WAVETYPE	"WAVEtype"

// leaf nodes
//syst/
#define RESET		"RESEt" // not yet implemented
//source/frequency/
#define START		(String)"STARt"
#define STOP		"STOP"
#define SCALE		"SCALe"
#define HOLD		"HOLD"
#define INTERVAL	"INTErval"
#define SWEEP		"SWEEp"
//source/
#define VOLTAGE		"VOLTage"
#define WAVETYPE	"WAVEtype"

class Menu
{
public:
	Menu();	// constructor
	
	bool err;
	bool qry;

	void setSource();

	void validateNode(String&, bool&, bool&, bool&, bool&, char&, char&);
	void error(int, String, String);	// print requested error statement
	void reset();
	void setRoot(String&);
	void goToMenu(String&); // call menu for node
	void query(String&);	
	void assign(String&);
	
	// output menu
private:
	
	String menuRoot;
	String menuNode;
	String menuLeaf;

};