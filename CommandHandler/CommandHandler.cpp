// CommandHandler.cpp by Anuraag Narang

#include "Arduino.h"
#include "CommandHandler.h"
#include "Menu.h"
#include "SPI.h"

Menu mn; // create menu object

// constructor
CommandHandler::CommandHandler()
{
}

// boot instrument
void CommandHandler::boot()
{
	// Arduino
	SPI.begin();		// initialise SPI bus

	Serial.begin(9600);	// open serial port once instrument has been setup

	reset();			// reset CommandHandler parameters
	mn.setSource();	// initialise function generator parameters
}

// reset flags
void CommandHandler::reset()
{
	rdy			= true;							// ready to receive commands
	root_node	= false;						// root not set
	mid_node	= false;						// node not set
	leaf_node	= false;						// leaf not set
	val_node	= false;						// value not set
	front		= false;						// start marker not
	back   		= false;						// stop marker not set
	//end_cmmd	= false;
	Serial.println(F("<INSTRUMENT IS READY>"));	// status message
}

// decode and implement a command
void CommandHandler::processCommand() 
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
			
			if (tempNode == VOLTAGE || tempNode == WAVETYPE)
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
		else if (mn.qry == false && val_node == false)
		{
			tempNode = readNode();	// get param value
			setNode(tempNode, val_node);
			mn.assign(tempNode);	// set param
			
			// reset command handler flags
			reset();
			mn.reset();
		}
	}

	// query the parameter
	else if (mn.qry == true)
	{
		// call query method in mn
		mn.query(tempNode);

		// reset command handler flags
		reset();
		mn.reset();
	}
}

// read the next node in the command
String CommandHandler::readNode()
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
void CommandHandler::setNode(String& node, bool& node_flag)
{
	mn.validateNode(tempNode, root_node, mid_node, leaf_node, val_node, frontMarker, backMarker);
	
	if (mn.err == true)
	{
		node_flag = false;
		Serial.println(F("Node not set"));
		reset();
		mn.reset();
	}
	else
	{
		node_flag = true;

		String nodeName;
		
		if		(val_node == true)	{ nodeName = "Value"; }
		else if (leaf_node == true) { nodeName = "Leaf"; }
		else if (mid_node == true)	{ nodeName = "Node"; }
		else if (root_node == true)
		{
			nodeName = "Root";
			rdy = false;
			//if (rdy == false) { Serial.println(F("Not ready")); }
		}
		Serial.println(nodeName + " set to " + tempNode);

		frontMarker = backMarker;
		backMarker = ';';
		back = false;
	}
}