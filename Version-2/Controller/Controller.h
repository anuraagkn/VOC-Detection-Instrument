//#include "Arduino.h"

#define NODE_SIZE		(int)32

class Controller
{
public:
	Controller();
	void boot(); // boot instrument
	void reset(); // reset instrument
	void processCommand();
	String readNode();
	void setNode(String&, bool&);
private:
	// flags
	bool rdy;		// ready
	bool root_node; // root node set
	bool mid_node;	// node set
	bool leaf_node;	// leaf node set
	bool val_node;	// value set
	bool front;		// front marker set
	bool back;		// back marker set

	char node[NODE_SIZE];
	String tempNode;
	const char beginMarker	= ':';
	const char endMarker	= ';';
	const char spaceMarker	= ' ';
	const char openMarker	= '{';
	const char closeMarker	= '}';
	const char queryMarker	= '?';
	char frontMarker;
	char backMarker;
};