// Controller.cpp by Anuraag Narang

#include "Arduino.h"
#include "Controller.h"
#include "Menu.h"
#include "SPI.h"

Menu menu; // create menu object

Controller::Controller()
{
	// initialise flags
	rdy = true;			// ready to receive commands
	proc_cmmd = false;	// receiving command (not ready)
	root_node = false;	// root node set
	end_cmmd = false;
}

// boot instrument
void Controller::boot()
{
	SPI.begin();		
	Serial.begin(9600);							// open serial port
	Serial.println(F("<INSTRUMENT IS READY>")); // status message
}

// reset all global variables
void Controller::reset()
{
	rdy = true;			// ready to receive commands
	proc_cmmd = false;	// receiving command (not ready)
	root_node = false;	// root node set
	end_cmmd = false;
	//memset(&node[0], 0, sizeof(node)); // clear node array
	// also need to reset menu
}

// decode and implement a command
void Controller::processCommand() 
{
	if (Serial.available() > 0)
	{
		delayMicroseconds(5000);
		if (root_node == false) // set root node
		{

			// read root node
			checkBegin();									// check for ':' at start of command
			//delayMicroseconds(1500);
			rootNode = readUntil(beginMarker, root_node);	// read root node and set flag
			if (end_cmmd == true) 
			{
				Serial.println(F("Error: End of command reached unexpectedly after root node"));
				reset();
			}
			else
			{
				// only call setRoot if root node has been read and end of command not reached
				if (root_node == true) { menu.setRoot(rootNode); rdy = false; }
				if (rdy == false) { Serial.println(F("Processing command")); } // test: ready flag cleared


			}
		}
		else
		{
			//
		}
	}
}

// check for the start of a command
void Controller::checkBegin()
{
	//Serial.println("rdy")
	char rc;

	if (rdy == true && Serial.available() > 0)
	{	
		Serial.println(F("checking command"));
		rc = Serial.read();														// read next char from serial buffer
		switch (rc)																// check if char is valid
		{
		case ':':	proc_cmmd = true; rdy = false;								// processing command so not ready to receive another
					Serial.println("First marker is " + String(rc));
					if (proc_cmmd == true) { Serial.println(F("Command begun")); }
					break;
		default:	Serial.println("Error: Invalid command begin marker \"" + String(rc) + "\"");//rdy=true; // char != startMarker so command is invalid
					rdy = true; proc_cmmd = false;								// maintain flags
		}
	}
}

String Controller::readUntil(const char& marker, bool& flag)
{
	int ndx = 0;
	char rc;
	bool mark = false; // marker flag

	while (Serial.available() > 0 && flag == false && proc_cmmd == true)
	{	
		rc = Serial.read(); // read next char

		if (rc != beginMarker && rc != endMarker && rc != spaceMarker && rc != openMarker && rc != closeMarker)
		{
			node[ndx] = rc;
			Serial.println("Writing to ndx" + String(ndx));
			ndx++;
			Serial.println(String(rc));
			//mark = false;

			if (ndx > NODE_SIZE) { ndx = NODE_SIZE - 1; } // handle overflow
		}
		else
		{
			node[ndx] = '\0'; // terminate string
			ndx = 0; // reset index
			Serial.println("rc = " + String(rc));
			mark = true;
			Serial.println(F("Mark set"));
			if (rc == marker)
			{ 
				String temp_node = String(node);
				Serial.println("Node is " + temp_node);
				flag = true; // set flag
				end_cmmd = false;	// marker reached therefore not end of command
			}
			else
			{
				//error
			}
		}
	}

	// check for end of command
	if (mark == false)
	{
		end_cmmd = true;
		Serial.println(F("End of command"));
	}

	return String(node);
}