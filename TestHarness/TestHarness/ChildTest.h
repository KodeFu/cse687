//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// ChildTest.h : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/22/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	David L Howick			Original
// 4/23/2020	Troy H Archer			Adding initial content
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <string>
#include "Logger.h"

// Typedef the function pointer
typedef bool(__stdcall* testFunc)();

class ChildTest
{
public:
	ChildTest();										// Constructor
	ChildTest(Logger* logger);							// Constructor
	~ChildTest();										// Destructor
	bool test(testFunc testToExecute);					// Test with passed in function pointer

private:
	//Logger pointer
	Logger* mLogger;
};
