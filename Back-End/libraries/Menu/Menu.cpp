#include "Arduino.h"
#include "Menu.h"
#include "Source.h"

Source src;

Menu::Menu()
{
	err = false;
	qry = false;
}

void Menu::setSource()
{
	src.AD9833Reset();

	// set function generator defaults
	src.setFreq(START, String(1000.0));
	src.setAmp(String(10.0));
	src.setWave(String("TRIangular"));

	// sweep defaults
	src.setFreq(STOP, String(10000.0)); // 10kHz
	src.setScale(String("LIN"));	// linear scale
	src.setPoints(String(9.0));	// 9 frequencies
	src.setHold(String(2));		// 2 seconds per frequency (1000 * 10 milliseconds)
	menuNode = " "; // default mid node in case of two-node command
}

void Menu::reset()
{
	err = false;
	qry = false;
}

void Menu::validateNode(String& name, bool& root, bool& mid, bool& leaf, bool& val,
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
		else if (name == FREQUENCY || name == PUMP)
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
		else if ((menuNode == FREQUENCY && (name == START || name == STOP || name == SCALE || name == HOLD || name == INTERVAL || name == PPP
			|| name == SWEEP || name == SWE))
			|| (menuNode == PUMP && (name == VOLUME || name == TIME))
			|| (menuNode == " " && (name == VOLTAGE || name == WAVETYPE)))
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

	// validade value node
	else if (val == false)
	{
		Serial.println(F("Validating leaf node"));
		if (start != ' ')
		{
			error(1, menuNode, " ");
		}
		else if (stop != ';')
		{
			error(2, "", name);
		}
		// else value node syntax is valid
	}
}

void Menu::query(String& param)
{
	Serial.println("Querying " + param);



	qry = false;
}

void Menu::assign(String& val)
{
	Serial.println("Assigning " + val);

	if (menuLeaf == START) { src.setFreq(START, val); }
	else if (menuLeaf == VOLTAGE) { src.setAmp(val); }
	else if (menuLeaf == WAVETYPE) { src.setWave(val); }
	else if (menuLeaf == STOP) { src.setFreq(STOP, val); }
	else if (menuLeaf == SCALE) { src.setScale(val); }
	else if (menuLeaf == HOLD) { src.setHold(val); }
	else if (menuLeaf == INTERVAL) { src.setPoints(val); }
	else if (menuLeaf == SWEEP || menuLeaf == SWE) { src.sweep(val); }
	else if (menuLeaf == VOLUME || menuLeaf == VOL) { src.setPumpFlow(val); }
	else if (menuLeaf == TIME) { src.setPumpTime(val); }
	//else {} error invalid command
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
