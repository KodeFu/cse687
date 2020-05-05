//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// ChildTest.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/23/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/23/2020	Troy H Archer			Adding initial content
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <iostream>
#include <exception>
#include "ChildTest.h"



ChildTest::ChildTest()
{

}


ChildTest::ChildTest(Logger* logger) :
	mLogger(logger)
{

}

ChildTest::~ChildTest()
{

}

bool ChildTest::test(testFunc testToExecute)
{
	//Attempt to run the test
	try {
		//Get the result of the test we're executing
		bool testResult = testToExecute();

		if (testResult)
		{
			mLogger->log(Logger::INFO, "Test = true!");
			return true;
		}
		else
		{
			mLogger->log(Logger::ERROR_E, "Test = false!");
			return false;
		}
	}
	catch (const std::exception& e)
	{
		mLogger->log(Logger::ERROR_E, "Exception!");
	}

	//If it hasn't returned true already, return false
	return false;
}