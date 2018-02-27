
// root nodes
#define SYSTEM		"SYSTem"	// unused
#define SOURCE		"SOURce"

// nodes
	//source/
#define	FREQUENCY	"FREQuency"
#define WAVETYPE	"WAVEtype" // Don't need this?
#define PUMP		"PUMP"

// leaf nodes
//syst/
#define RESET		"RESEt" // not yet implemented
//source/frequency/
#define START		"STARt"
#define STOP		"STOP"
#define SCALE		"SCALe"
#define HOLD		"HOLD"
#define INTERVAL	"INTErval"
#define PPP			"PPPOINt" // not yet implemented
#define SWEEP		"SWEEP"
#define SWE         "SWE"
//source/
#define VOLTAGE		"VOLTage"
#define WAVETYPE	"WAVEtype"
//source/pump/
#define VOLUME		"VOLUme" // not yet implemented
#define VOL			"VOL"
#define TIME		"TIMEr" // not yet implemented

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
private:

	String menuRoot;
	String menuNode;
	String menuLeaf;

};
