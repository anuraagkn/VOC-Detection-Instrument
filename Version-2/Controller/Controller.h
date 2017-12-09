#include "Arduino.h"

#define NODE_SIZE		(int)32

class Controller
{
public:
	Controller();
	void boot(); // boot instrument
	void reset(); // reset instrument
	void checkBegin();
	String readUntil(const char&, bool&);
	void processCommand();
	String readNode();
private:
	bool rdy;	// ready flag
	//bool qry; // query flag
	bool end_cmmd; // end of command flag
	/*proc_cmmd is void w.r.t. functionality. Get rid of it*/
	bool root_node; // flag: root node set
	bool mid_node;	// mid node flag
	bool leaf_node;	// flag: leaf node set
	bool val_node;
	bool m1;
	bool m2;
	char node[NODE_SIZE];
	String rootNode;
	String midNode;
	String leafNode;
	String tempNode;
	const char beginMarker	= ':';
	const char endMarker	= ';';
	const char spaceMarker	= ' ';
	const char openMarker	= '{';
	const char closeMarker	= '}';
	const char queryMarker	= '?';
	char startMarker;
	char stopMarker;
};