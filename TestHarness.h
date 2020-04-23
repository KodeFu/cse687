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

class TestHarness
{
public:
	TestHarness();										// Constructor
	~TestHarness();										// Destructor
	void createThreadPool();							// Create and instantiate thread pool of Child Test objects
	bool executor();									// Accept callable object (no args) ret. bool
	void parser();										// Parse the request into individual tests.  Assign to
														// single child test thread (each test)
	void loadLibraries();								// Create a log queue
	void getMessage();									// Receive messages

private:
	
};
