// Controller.cpp by Anuraag Narang

#include "Arduino.h"
#include "Controller.h"
#include "Menu.h"
#include "SPI.h"

Menu menu; // create menu object

Controller::Controller()
{
	// initialise flags
	rdy			= true;			// ready to receive commands
	root_node	= false;	// root node set
	mid_node	= false;
	leaf_node	= false;
	end_cmmd	= false;
	m1			= false;
	m2			= false;
}

// boot instrument
void Controller::boot()
{
	SPI.begin();		
	Serial.begin(9600);							// open serial port
	Serial.println(F("<INSTRUMENT IS READY>")); // status message
}

// reset flags
void Controller::reset()
{
	rdy			= true;			// ready to receive commands
	root_node	= false;	// root node set
	mid_node	= false;
	leaf_node	= false;
	val_node	= false;
	end_cmmd	= false;
	m1			= false;
	m2			= false;
	Serial.println(F("<INSTRUMENT IS READY>"));
}

// decode and implement a command
void Controller::processCommand() 
{
	
	if (Serial.available() > 0)
	{
		delayMicroseconds(25000); // allow time for entire command to be received by Arduino
		// set root node
		if (root_node == false)
		{
			Serial.println(F("Reading root node"));
			//rootNode = readNode(); // read root node
			tempNode = readNode(); // read root node
			menu.validateNode(tempNode, root_node, mid_node, leaf_node, startMarker, stopMarker);
			if (menu.err == true) 
			{
				root_node = false;
				reset();
				Serial.println(F("Root not set"));
			}
			else
			{
				rdy = false;
				root_node = true;
				Serial.println("Root set to " + tempNode);
				startMarker = stopMarker;
				m2 = false;
			}
		}

		// set next node
		else if (mid_node == false)
		{
			Serial.println(F("Reading node"));
			//midNode = readNode(); // read node
			tempNode = readNode(); // read node
			
			if (tempNode == "VOLTage" || tempNode == "WAVEtype")
			{
				//leafNode = midNode; // read node
				//tempNode = midNode;
				mid_node = true;
				menu.validateNode(tempNode, root_node, mid_node, leaf_node, startMarker, stopMarker);
				if (menu.err == true)
				{
					leaf_node = false;
					reset();
					Serial.println(F("Leaf not set"));
				}
				else
				{
					leaf_node = true;
					Serial.println("Leaf set to " + tempNode);
					startMarker = stopMarker;
					m2 = false;
				}
			}
		
			else
			{
				Serial.println(F("other"));
				menu.validateNode(tempNode, root_node, mid_node, leaf_node, startMarker, stopMarker);
				if (menu.err == true)
				{
					mid_node = false;
					reset();
					Serial.println(F("Node not set"));
				}
				else
				{
					mid_node = true;
					Serial.println("Node set to " + tempNode);
					startMarker = stopMarker;
					m2 = false;
				}
			}
		}
		
		// set leaf node
		else if (leaf_node == false)
		{
			Serial.println(F("Reading leaf node 2"));
			//leafNode = readNode(); // read node
			tempNode = readNode(); // read leaf node
			menu.validateNode(tempNode, root_node, mid_node, leaf_node, startMarker, stopMarker);
			if (menu.err == true)
			{
				leaf_node = false;
				reset();
				Serial.println(F("Node not set"));
			}
			else
			{
				leaf_node = true;
				Serial.println("Leaf set to " + tempNode);
				startMarker = stopMarker;
				m2 = false;
			}
		}
		else if (menu.qry == false && val_node == false)
		{
			tempNode = readNode();	// get param value
			menu.assign(tempNode);	// set param
			
			// reset command handler flags
			reset();
			menu.reset();
		}

		// end of serial command
	}
	else if (menu.qry == true)
	{
		Serial.println(F("Querying parameter"));
		menu.qry = false;

		// reset command handler flags
		reset();
		menu.reset();
	}
}

String Controller::readNode()
{
	int ndx = 0;	// node name array index
	char rc;		// latest read char
	//char node[NODE_SIZE];
	// read node
	while (Serial.available() > 0 && m2 == false)
	{	
		rc = Serial.read(); // read next char
		//Serial.println("Char =  " + String(rc));
		// if char is not a marker, add to array
		if (rc != beginMarker && rc != endMarker && rc != spaceMarker && rc != queryMarker && rc != openMarker && rc != closeMarker)
		{
			node[ndx] = rc;
			//Serial.println("Writing to ndx" + String(ndx)); // test
			ndx++;
			//Serial.println(String(rc)); // test

			if (ndx > NODE_SIZE) { ndx = NODE_SIZE - 1; } // handle overflow
		}
		else
		{
			node[ndx] = '\0'; // terminate string
			ndx = 0; // reset index
			if (m1 == false) { startMarker = rc; m1 = true; }
			else if (m2 == false) { stopMarker = rc; m2 = true; }
		}
	}

	// clear array at end of command
	if (Serial.available() == 0 && rc != queryMarker) 
	{ 
		node[ndx] = '\0'; ndx = 0; 
	}

	Serial.println("Node has been read as " + String(node));
	return String(node);
}
