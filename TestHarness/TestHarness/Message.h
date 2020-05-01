//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/22/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	David L Howick			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <string>

using std::string;

class Message
{
public:
	Message();											// Constructor
	~Message();											// Destructor

private:
	string sourceAddr;									// Source Address
	string destAddr;									// Destination Address
	string messageType;									// Message Type
	string timeDateStamp;								// Time Date Stamp
	string author;										// Message author
	string message;										// XML or JSON string
};