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

class TestExecutive
{
public:
	TestExecutive();									// Constructor
	~TestExecutive();									// Destructor
	void createHarness();								// Create and instantiate the TestHarness Object
	void createGUI();									// Create and instantiate a GUI
	void createTestQueue();								// Create the Test queue of TestRequests
	void createLogQueue();								// Create a log queue
	void testRequestMessage();							// Test request message with XML/JSON string

private:
	int testRequests;
};
