//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: This file contains the 'main' function. Program execution begins and ends there. See file
//		Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete project requirements,
//		architecture, and design.
// Parameters:  None
// Create Date:  4/22/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	David L Howick			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <windows.h>
#include "TestExecutive.h"
#include "TestHarness.h"
#include "TestRequestQueue.h"
#include "LogOutputQueue.h"
#include "ChildTest.h"
#include "ThreadMessageQueue.h"


TestHarness::TestHarness()
{
	mLogMsgQueue = new ThreadMessageQueue<Message>;
	mLogger = new Logger(mLogMsgQueue);
}

int main()
{
	// TestRequestQueue tq;
	ThreadMessageQueue<Message> tq;
	ThreadMessageQueue<Message>* logQueue = new ThreadMessageQueue<Message>;

	Message msg;
	
	// Since this class isn't object oriented at the moment, just make a local logger
	Logger* log = new Logger(logQueue);

	msg.filePath = "TestFuncsA.dll";
	msg.functionName = "Test1";

	tq.Enqueue(msg);

	msg.filePath = "TestFuncsA.dll";
	msg.functionName = "Test2";
	tq.Enqueue(msg);

	msg.filePath = "TestFuncsA.dll";
	msg.functionName = "Test4";
	tq.Enqueue(msg);

	// While (Hardcoded to be true for now)
	while (true)
	{
		// Grab the first message off the queue, this is a blocking call
		Message msgD = tq.Dequeue();

		// Declare string variables for holding DLL and Function Name
		std::string testDllName = msgD.filePath;
		std::string testDllFuncName = msgD.functionName;;
		std::wstring wTestDllName(testDllName.begin(), testDllName.end());
		LPCWSTR lpcwstrTestDllName = wTestDllName.c_str();
		LPCSTR lpcstrTestDllFuncName = testDllFuncName.c_str();

		// Instantiate the the child test object
		ChildTest childTest(log);

		// Get a handle to the test dll
		HINSTANCE hTestDll = LoadLibrary(lpcwstrTestDllName);
		if (hTestDll != NULL)
		{
			lpcstrTestDllFuncName = testDllFuncName.c_str();
			testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, lpcstrTestDllFuncName);
			if (NULL != pTestFunc)
			{
				// Give the function pointer to the child test object
				childTest.test(pTestFunc);
			}
			else
			{
				std::cout << "TestHarness: " << testDllName << " Couldn't find test function." << std::endl;
			}
			// Free up the DLL after we're done using it
			FreeLibrary(hTestDll);
		}
		else
		{
			std::cout << "TestHarness: Could Not find DLL: " << testDllName << std::endl;
		}
	}
}

