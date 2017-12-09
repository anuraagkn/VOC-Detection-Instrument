#include "Arduino.h"
#include "Menu.h"


Menu::Menu()
{
	err = false;
	qry = false;
	//sys = false;
	//src = false;
}

void Menu::reset()
{
	err = false;
	qry = false;
}

bool Menu::validateNode(String& name, bool& root, bool& mid, bool& leaf,
						char& start, char& stop)
{	
	// validate root node
	if (root == false)
	{
		Serial.println(F("Validating root node"));
		if (start != ':')
		{
			error(1, ":", "");
		}
		else if (name == SYSTEM || name == SOURCE) // check for other possible roots here
		{
			//Serial.println("Name = " + name); //test
			// validate start marker
			if (stop != ':')
			{
				error(2, ":", name);
			}
			else { menuRoot = name; } // set root
		}
		// invalid root name
		else
		{
			error(3, "", name);
		}
	}

	// validate node
	else if (mid == false)
	{
		Serial.println(F("Validating node"));
		// for the valid root name
		if (start != ':')
		{
			error(1, menuRoot, ":");
		}
		else if (name == FREQUENCY)
		{
			if (stop != ':')
			{
				error(2, ":", name);
			}
			else { menuNode = name; } // set node
		}
		
		else { error(3, "", name); }
	}
	
	// validate leaf node
	else if (leaf == false)
	{
		Serial.println(F("Validating leaf node"));
		if (start != ':')
		{
			error(1, menuNode, ":");
		}
		else if (name == START || name == STOP || name == SCALE || name == STEP || name == INTERVAL || name == VOLTAGE || name == WAVETYPE)
		{
			if (stop != ' ' && stop != '?')
			{
				Serial.println("pad" + String(stop) + "pad");
				error(2, " \" or \"?", name);
			}
			else
			{
				menuLeaf = name;
				if (stop == '?') { qry = true; Serial.println(F("qry set")); }
			}
		}
		else { error(3, "", name); }
	}
}

void Menu::query() {}

void Menu::assign(String& val)
{
	Serial.println(val + " has been assigned");

}

// error statements
void Menu::error(int s, String m, String n)
{
	err = true;
	Serial.println(F(""));

	// select the appropriate error statement
	switch (s) 
	{
		case 1: Serial.println("Error: Expected \"" + m + "\" before node \"" + n + "\""); break;
		case 2: Serial.println("Error: Expected \"" + m + "\" after node \"" + n + "\""); break;
		case 3: Serial.println("Error: Invalid node name \"" + n + "\""); break;
		//case 4: Serial.println(F("")); break;
		//case 5: Serial.println(F("Error: ")); break;
		default: Serial.println(F("Error: Invalid command.")); break;
	}
	
}
