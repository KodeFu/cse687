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

// prototype for the test functions
typedef bool(__stdcall* testFunc)();

ChildTest::ChildTest()
{
	testDllName = "TestRequest01.DLL";
	testDllFuncName = "Test1";
}


ChildTest::ChildTest(Logger* logger) :
	mLogger(logger)
{
	testDllName = "TestRequest01.DLL";
	testDllFuncName = "Test1";
}

ChildTest::~ChildTest()
{

}

void ChildTest::setTestDllFuncName(std::string testName)
{
	testDllFuncName = testName;
}

bool ChildTest::test()
{
	bool result = false;
	std::wstring wTestDllName(testDllName.begin(), testDllName.end());
	LPCWSTR lpcwstrTestDllName = wTestDllName.c_str();
	LPCSTR lpcstrTestDllFuncName;

	// get a handle to the test dll
	HINSTANCE hTestDll = LoadLibrary(lpcwstrTestDllName);
	if (hTestDll != NULL)
	{
		// get a pointer to the function
		// testDllFuncName = tq.dequeueTest();
		lpcstrTestDllFuncName = testDllFuncName.c_str();
		testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, lpcstrTestDllFuncName);
		if (NULL != pTestFunc)
		{
			// run the function
			result = pTestFunc();
			std::cout << "TestHarness: " << testDllName << " Test Function: " << testDllFuncName
				<< " returned " << (result ? "True" : "False") << " in the Child Test Class." << std::endl;
		}
		else
		{
			std::cout << "TestHarness: " << testDllName << " Couldn't find test function." << std::endl;
			result = false;
		}

		FreeLibrary(hTestDll);
		return result;
	}
	else
	{
		std::cout << "TestHarness: Could Not find DLL: " << testDllName << std::endl;
		result = false;
		return result;
	}
}

bool ChildTest::test(bool(*testToExecute)())
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