#include "Arduino.h"

#define NODE_SIZE		(int)32

class Controller
{
public:
	bool qry; // query flag
	bool end_cmmd; // end of command flag
	Controller();
	void boot(); // boot instrument
	void reset(); // reset instrument
	void checkBegin();
	String readUntil(const char&, bool&);
	void processCommand();
private:
	bool rdy;	// ready flag
	bool proc_cmmd; // receiving command flag
	/*proc_cmmd is void w.r.t. functionality. Get rid of it*/
	bool root_node; // read root node flag
	char node[NODE_SIZE];
	String rootNode;
	const char beginMarker	= ':';
	const char endMarker	= ';';
	const char spaceMarker	= ' ';
	const char openMarker	= '{';
	const char closeMarker	= '}';
};