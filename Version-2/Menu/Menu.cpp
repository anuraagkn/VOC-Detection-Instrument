#include "Arduino.h"
#include "Menu.h"


Menu::Menu()
{
	sys = false;
	src = false;
}

void Menu::setRoot(String& r)
{
	/*if (r == SOURCE)
	{
		root = SOURCE;
		Serial.println("Root set to " + root);
	}
	else 
	{
		Serial.println("Error: Invalid root node \"" + r + "\"");
	}*/

	if (r != SYST && r != SOURCE) // invalid root node
	{
		Serial.println("Error: Invalid root node \"" + r + "\"");
	}
	else if (r == SYST) // set root to system
	{
		root = SYST;
	}
	else if (r == SOURCE) // set root to source
	{
		root = SOURCE;
	}

	Serial.println("Root set to " + root);

}

/*

sourceMenu(){

	

}

*/