/* Menu.cpp by Anuraag Narang

This class validates nodes in recieved commands.
Once a leaf node has been processed, the value is passed to the appropriate function.

*/

#include "Arduino.h"
#include "Menu.h"
#include "System.h"

System sys; // system object

Menu::Menu()
{
	err = false;
	qry = false;
}

void Menu::setInstrument()
{/*
	src.AD9833Reset();
	// set function generator defaults
	src.setFreq(START, String(1000.0));
	src.setAmp(String(3));
	src.setWave(String("TRIangular"));
    src.setLamp(String(100));       // default lamp brightness level
    // auto experiment defaults
	src.setFreq(STOP, String(10000.0)); // 10kHz
	src.setScale(String("LIN"));	// linear scale
	src.setPoints(String(9.0));	// 9 frequencies
	src.setHold(String(2));		// 2 seconds per frequency (1000 * 10 milliseconds)
	src.setPumpFlow(String(3));         // pump flow
	src.setPumpTime(String(5));         // pump timer
	src.setPPPoint("N");        // pump per frequency point

	mtr.reset(src.freq, src.hold);
	*/
	sys.reset(); // reset system
	menuNode = " "; // default mid node in case of two-node command
}

void Menu::reset()
{
	err = false;
	qry = false;
	menuNode = " "; // need to reset menu node otherwise when setting voltage/wavetype, menuNode will not be read as " ".
}

void Menu::validateNode(String& name, bool& root, bool& mid, bool& leaf, bool& val,
						char& start, char& stop)
{
	// validate root node
	if (root == false)
	{
		if (start != ':'){ error(1, ":", ""); }
		else if (name == SYSTEM || name == SYST || name == SOURCE/* || name == METER*/) // check for other possible roots here
		{
			// validate start marker
			if (stop != ':'){ error(2, ":", name); }
			else { menuRoot = name; } // set root
		}
		// invalid root name
		else{ error(3, "root", name); }
	}
	// validate node
	else if (mid == false)
	{
		if (start != ':'){ error(1, menuRoot, ":"); }
		else if (
					((menuRoot == SYSTEM || menuRoot == SYST) && // system conditions
						(name == AUTOMATIC || name == AUTO )
					)
					|| ((menuRoot == SOURCE) && (name == PUMP) // source conditions
					)// ***WHAT ABOUT METER***
				)// end if conditions
		{
			if (stop != ':'){ error(2, ":", name); }
			else { menuNode = name; } // set node
		}

		else { error(3, "mid", name); }
	}
	// validate leaf node
	else if (leaf == false)
	{
		if (start != ':'){ error(1, menuNode, ":"); }
		else if (
			((menuRoot == SYSTEM  || menuRoot == SYST) &&  // system conditions
				((menuNode == " " && (name == RESET))
				|| ((menuNode == AUTOMATIC || menuNode == AUTO) && 
					(name == STARTFREQUENCY || name == STARTFREQ || name == STOPFREQUENCY || name == STOPFREQ || name == SCALE
					|| name == HOLD || name == POINTS || name == PPP || name == EXPERIMENT || name == EXP))
				)
			)						// end system conditions
			|| (menuRoot == SOURCE &&	// begin source conditions
				((menuNode == " " && (name == FREQUENCY || name == FREQ || name == VOLTAGE || name == VOLT
				|| name == WAVETYPE || name == WAVE || name == LAMP))
				|| ((menuNode == PUMP) && 
				(name == VOLUME || name == VOL || name == TIME || name == POWER || name == POW)
				)
				)
			)							// end source conditions
			//***ADD METER CONDITIONS***
			) // end if conditions
		{
			if (stop != ' ' && stop != '?')
			{
				//Serial.println("pad" + String(stop) + "pad"); // test
				error(2, " \" or \"?", name);
			}
			else
			{
				menuLeaf = name;
				if (stop == '?') { qry = true; ; }//Serial.println(F("qry set")); }
			}
		}
		else { error(3, "leaf", name); }
	}
	// validade value node
	else if (val == false)
	{
		if (start != ' '){ error(1, menuNode, " "); }
		else if (stop != ';'){ error(2, "", name); }
	}
}

// currently not used for any purpose.
void Menu::query(String& param)
{
	//Serial.println("Querying " + param);
	//if (menuLeaf == START ){src.getFreq(START);}
	qry = false;
}

void Menu::select(String& val)
{
	int actionid;

	// determine action number
	if (menuLeaf == RESET) { actionid = 1; }
	else if (menuLeaf == STARTFREQUENCY || menuLeaf == STARTFREQ)	{ actionid = 2; }
	else if (menuLeaf == STOPFREQUENCY || menuLeaf == STOPFREQ)		{ actionid = 3; }
	else if (menuLeaf == SCALE) { actionid = 4; }
	else if (menuLeaf == POINTS){ actionid = 5; }
	else if (menuLeaf == HOLD)	{ actionid = 6; }
	else if (menuLeaf == PPP)	{ actionid = 7; }
	else if (menuLeaf == EXPERIMENT || menuLeaf == EXP)	{ actionid = 8; }
	else if (menuLeaf == FREQUENCY|| menuLeaf == FREQ)	{ actionid = 9; }
	else if (menuLeaf == VOLTAGE || menuLeaf == VOLT)	{ actionid = 10; }
	else if (menuLeaf == WAVETYPE || menuLeaf == WAVE)	{ actionid = 11; }
	else if (menuLeaf == LAMP)							{ actionid = 12; }
	else if (menuLeaf == VOLUME || menuLeaf == VOL)		{ actionid = 13; }
	else if (menuLeaf == TIME)							{ actionid = 14; }
	else if (menuLeaf == POWER || menuLeaf == POW)		{ actionid = 15; }
	else { actionid = 0; Serial.println("Error in selcting method"); } // Default: do nothing
	
	sys.enact(actionid, val); // specify action for system to take
}

// error statements
void Menu::error(int s, String m, String n)
{
	err = true;
	//Serial.println(F(""));

	// select the appropriate error statement
	switch (s)
	{
		case 1: Serial.println("Error: Expected \"" + m + "\" before node \"" + n + "\""); break;
		case 2: Serial.println("Error: Expected \"" + m + "\" after node \"" + n + "\""); break;
		case 3: Serial.println("Error: Invalid node name \"" + n + "\"" + "(" + m + "), root node is " + menuRoot); break;
		//case 4: Serial.println(F("")); break;
		//case 5: Serial.println(F("Error: ")); break;
		default: Serial.println(F("Error: Invalid command.")); break;
	}
}
