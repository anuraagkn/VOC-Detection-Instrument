#include "NodeList.h" // list of command nodes

class Menu
{
public:
	Menu();	// constructor

	bool err;
	bool qry;
	
	void setInstrument();
	void reset();
	void validateNode(String&, bool&, bool&, bool&, bool&, char&, char&);
	void query(String&);
	void select(String&);
	void error(int, String, String);	// print requested error statement
private:

	String menuRoot;
	String menuNode;
	String menuLeaf;

};
