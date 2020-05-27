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

#include "ProcessMessageQueue.h"
#include <iostream>
#include <string>
#include <thread>
#include "TestHarness.h"
#include "ThreadMessageQueue.h"

using namespace std;

#define THREAD_POOL_MAX 5 // number of threads in the thread pool

// childTester function
void childTester(ProcessMessageQueue& testQueue);

// prototype for the test functions
typedef bool(__stdcall* testFunc)();


int main()
{
	// wait for TestExec - do this in another thread?
	cout << "Waiting for TestExec() to connect.... " << endl;
	ProcessMessageQueue testQueue;
	testQueue.ServerListen("127.0.0.1", 5005);

	// Since this class isn't object oriented at the moment, just make a local logger
	//ProcessMessageQueue logQueue;
	//Logger* log = new Logger(logQueue);

	for (int i = 0; i < THREAD_POOL_MAX; i++)
	{
		std::thread{ childTester, std::ref(testQueue) }.detach();
	}
	
	cout << "return" << endl;
	cin.get();

}

void childTester(ProcessMessageQueue& testQueue)
{
	cout << "(" << GetCurrentThreadId() << ") " "started" << endl;

	while (true)
	{
		// Grab the first message off the queue, this is a blocking call
		//cout << GetCurrentThreadId() << ": waiting for test..." << endl;
		if (testQueue.isEmpty())
		{
			Sleep(1000);
			continue;
		}

		Message msg = testQueue.Dequeue();

		cout << "(" << GetCurrentThreadId() << ") " << "running test" << endl;

		// Declare string variables for holding DLL and Function Name
		std::string testDllName = msg.filePath;
		std::string testDllFuncName = msg.functionName;;

		cout << "(" << GetCurrentThreadId() << ") " << testDllName << endl;
		cout << "(" << GetCurrentThreadId() << ") " << testDllFuncName << endl;

		// Instantiate the the child test object
		//ChildTest childTest(NULL); // childTest(log);
		
		// get a handle to the test dll
		HINSTANCE hTestDll = LoadLibraryA(testDllName.c_str());
		if (hTestDll != NULL)
		{
			testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, testDllFuncName.c_str());
			if (NULL != pTestFunc)
			{
				try
				{
					// call the test function
					pTestFunc();
				}
				catch (...)
				{
					//mLogger->log(Logger::ERROR_E, "Exception!");
					cout << "(" << GetCurrentThreadId() << ") " << "exception!" << endl;
				}
			}
			else
			{
				std::cout << "(" << GetCurrentThreadId() << ") " << testDllName << " Couldn't find test function." << std::endl;
			}

			// free up the DLL after we're done using it
			cout << "(" << GetCurrentThreadId() << ") " << "freeDLL" << endl;
			FreeLibrary(hTestDll);
		}
		else
		{
			std::cout << "(" << GetCurrentThreadId() << ") " << "TestHarness: Could Not find DLL: " << testDllName << std::endl;
		}
	}
}