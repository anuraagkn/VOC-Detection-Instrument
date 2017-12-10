// Controller.cpp by Anuraag Narang

#include "Arduino.h"
#include "Controller.h"
#include "Menu.h"
#include "SPI.h"

Menu menu; // create menu object

// constructor
Controller::Controller()
{
}

// boot instrument
void Controller::boot()
{
	SPI.begin();		
	Serial.begin(9600);							// open serial port
	reset();
	//Serial.println(F("<INSTRUMENT IS READY>")); // status message
}

// reset flags
void Controller::reset()
{
	rdy			= true;							// ready to receive commands
	root_node	= false;						// root not set
	mid_node	= false;						// node not set
	leaf_node	= false;						// leaf not set
	val_node	= false;						// value not set
	front		= false;						// start marker not
	back   		= false;						// stop marker not set
	Serial.println(F("<INSTRUMENT IS READY>"));	// status message
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
			tempNode = readNode(); // read root node
			setNode(tempNode, root_node);
		}

		// set next node
		else if (mid_node == false)
		{
			Serial.println(F("Reading node"));
			tempNode = readNode(); // read node
			
			if (tempNode == "VOLTage" || tempNode == "WAVEtype")
			{
				mid_node = true;
				setNode(tempNode, leaf_node);
			}
		
			else
			{
				setNode(tempNode, mid_node);
			}
		}
		
		// set leaf node
		else if (leaf_node == false)
		{
			Serial.println(F("Reading leaf node 2"));
			tempNode = readNode(); // read leaf node
			setNode(tempNode, leaf_node);
		}

		// read assign the parameter value
		else if (menu.qry == false && val_node == false)
		{
			tempNode = readNode();	// get param value
			menu.assign(tempNode);	// set param
			
			// reset command handler flags
			reset();
			menu.reset();
		}
	}

	// query the parameter
	else if (menu.qry == true)
	{
		// call query method in Menu
		menu.query(tempNode);

		// reset command handler flags
		reset();
		menu.reset();
	}
}

// read the next node in the command
String Controller::readNode()
{
	int ndx = 0;	// node name array index
	char rc;		// latest read char

	// read node
	while (Serial.available() > 0 && back == false)
	{	
		rc = Serial.read(); // read next char

		// if char is not a marker, add to array
		if (rc != beginMarker && rc != endMarker && rc != spaceMarker && rc != queryMarker && rc != openMarker && rc != closeMarker)
		{
			node[ndx] = rc;
			ndx++;

			if (ndx > NODE_SIZE) { ndx = NODE_SIZE - 1; } // handle overflow
		}
		else
		{
			node[ndx] = '\0'; // terminate string
			ndx = 0; // reset index
			if (front == false) { frontMarker = rc; front = true; }
			else if (back == false) { backMarker = rc; back = true; }
		}
	}

	// clear array at end of command
	if (Serial.available() == 0 && rc != queryMarker) 
	{ 
		node[ndx] = '\0'; ndx = 0; 
	}

	//Serial.println("Node read as " + String(node));
	return String(node);
}

// set the node in the menu
void Controller::setNode(String& node, bool& node_flag)
{
	menu.validateNode(tempNode, root_node, mid_node, leaf_node, frontMarker, backMarker);
	if (menu.err == true)
	{
		node_flag = false;
		reset();
		Serial.println(F("Root not set"));
	}
	else
	{
		node_flag = true;

		String nodeName;
		
		if (leaf_node == true) { nodeName = "Leaf"; }
		else if (mid_node == true) { nodeName = "Node"; }
		else if (root_node == true)
		{
			nodeName = "Root";
			rdy = false;
			//if (rdy == false) { Serial.println(F("Not ready")); }
		}
		Serial.println(nodeName + " set to " + tempNode);

		frontMarker = backMarker;
		back = false;
	}
}