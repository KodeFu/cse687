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
// #include "Message.h"
#include "Logger.h"
#include "LogOutputQueue.h"
#include "ChildTest.h"
#include "ThreadMessageQueue.h"

// prototype for the test functions
typedef bool(__stdcall* testFunc)();

int main()
{
	// TestRequestQueue tq;
	ThreadMessageQueue<Message> tq;
	Message msg;

	msg.filePath = "TestRequest01.dll";
	msg.functionName = "Test1";

	tq.Enqueue(msg);
	Message msgD = tq.Dequeue();
	// declare string variables for holding DLL and Function Name
	std::string testDllName = msgD.filePath;
	std::string testDllFuncName;
	std::wstring wTestDllName(testDllName.begin(), testDllName.end());
	LPCWSTR lpcwstrTestDllName = wTestDllName.c_str();
	LPCSTR lpcstrTestDllFuncName;

	// get a handle to the test dll
	HINSTANCE hTestDll = LoadLibrary(lpcwstrTestDllName);
	if (hTestDll != NULL)
	{
		// get a pointer to the function
		testDllFuncName = msgD.functionName;
		lpcstrTestDllFuncName = testDllFuncName.c_str();
		testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, lpcstrTestDllFuncName);
		if (NULL != pTestFunc)
		{
			// run the function
			bool result = pTestFunc();
			std::cout << "TestHarness: " << testDllName << " Test Function: " << testDllFuncName
				<< " returned " << (result ? "True" : "False") << "." << std::endl;
		}
		else
		{
			std::cout << "TestHarness: " << testDllName << " Couldn't find test function." << std::endl;
		}

		msg.filePath = "TestRequest01.dll";
		msg.functionName = "Test2";
		tq.Enqueue(msg);
		Message msgD = tq.Dequeue();
		testDllFuncName = msgD.functionName;
		lpcstrTestDllFuncName = testDllFuncName.c_str();
		pTestFunc = (testFunc)GetProcAddress(hTestDll, lpcstrTestDllFuncName);
		if (NULL != pTestFunc)
		{
			// run the function
			bool result = pTestFunc();
			std::cout << "TestHarness: " << testDllName << " Test Function: " << testDllFuncName
				<< " returned " << (result ? "True" : "False") << "." << std::endl;
		}
		else
		{
			std::cout << "TestHarness: " << testDllName << " Couldn't find test function." << std::endl;
		}

		msg.filePath = "TestRequest01.dll";
		msg.functionName = "Test3";
		tq.Enqueue(msg);
		msgD = tq.Dequeue();
		testDllFuncName = msgD.functionName;
		lpcstrTestDllFuncName = testDllFuncName.c_str();
		pTestFunc = (testFunc)GetProcAddress(hTestDll, lpcstrTestDllFuncName);
		if (NULL != pTestFunc)
		{
			// run the function
			bool result = pTestFunc();
			std::cout << "TestHarness: " << testDllName << " Test Function: " << testDllFuncName
				<< " returned " << (result ? "True" : "False") << "." << std::endl;
		}
		else
		{
			std::cout << "TestHarness: " << testDllName << " Couldn't find test function." << std::endl;
		}

		FreeLibrary(hTestDll);
	}
	else
	{
		std::cout << "TestHarness: Could Not find DLL: " << testDllName << std::endl;
	}

	ChildTest ct1;
	ct1.test();

	std::cout << "TestHarness: " << testDllName << " Child Test Function: "
		<< " returned " << (ct1.test() ? "True" : "False") << "." << std::endl;

	ct1.setTestDllFuncName("Test2");

	std::cout << "TestHarness: " << testDllName << " Child Test Function: "
		<< " returned " << (ct1.test() ? "True" : "False") << "." << std::endl;

}

