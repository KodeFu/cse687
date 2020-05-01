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

class ChildTest
{
public:
	ChildTest();										// Constructor
	ChildTest(Logger* logger);							// Constructor
	~ChildTest();										// Destructor
	bool test(bool(*testToExecute)());					// test
	bool test();										// test
	void setTestDllFuncName(std::string);

private:
	//Logger pointer, assuming that the Test Harness will create the logger and pass around the reference to it
	//instead of each child tester having it's own logger
	Logger* mLogger;
	std::string testDllName;
	std::string testDllFuncName;
};
