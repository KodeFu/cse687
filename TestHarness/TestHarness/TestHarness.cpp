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
#include "Shlwapi.h"
#include <iostream>
#include <string>
#include <thread>
#include <iomanip> 
#include <chrono>
#include "ThreadMessageQueue.h"
#include "Logger.h"

using namespace std;

#define MAX_BUFFER			256     // used for sprintfs
#define THREAD_POOL_MAX		5		// number of threads in the thread pool

// childTester function
void childTester(ProcessMessageQueue & testQueue, ProcessMessageQueue & logQueue);

// prototype for the test functions
typedef bool(__stdcall* testFunc)();

Logger* mLogger;

int main()
{
	// create test queue
	cout << GetCurrentThreadId() << ": waiting for TestExec() to connect... " << endl;
	ProcessMessageQueue testQueue;
	testQueue.ServerListen("127.0.0.1", 5005);

	// create log queue
	ProcessMessageQueue logQueue;
	logQueue.ServerListen("127.0.0.1", 5006);

	// Since this class isn't object oriented at the moment, just make a local logger
	//ProcessMessageQueue logQueue;
	mLogger = new Logger(&logQueue);

	for (int i = 0; i < THREAD_POOL_MAX; i++)
	{
		std::thread{ childTester, std::ref(testQueue), std::ref(logQueue) }.detach();
	}

	// wait for key press
	cin.get();

}

void childTester(ProcessMessageQueue& testQueue, ProcessMessageQueue& logQueue)
{
	Message logMsg;
	char buf[MAX_BUFFER];;

	// Show starting thread and ids
	cout << GetCurrentThreadId() << ": started thread" << endl;

	// Loop forever
	while (true)
	{
		// check if there is a new test to execute
		if (testQueue.isEmpty())
		{
			Sleep(1000);
			continue;
		}

		// Dequeue the test
		Message msg = testQueue.Dequeue();

		// Declare string variables for holding DLL and Function Name
		std::string testDllName = msg.filePath;
		std::string testDllFuncName = msg.functionName;

		// Need to get the filename from the path, so we can log it 
		// Note: the message queue buffer size is small, so we needed
		// to remove the path so there is no buffer overflow
		char testDllShortName[MAX_BUFFER] = { 0 };
		strncpy_s(testDllShortName, MAX_BUFFER, testDllName.c_str(), testDllName.size());
		PathStripPathA(testDllShortName);

		// Build string to log
		snprintf(buf, MAX_BUFFER, "%d: %s %s %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "start test.");

		// Log the start of the test
		mLogger->log(Logger::INFO, logMsg, buf);

		// Get a handle to the test dll
		HINSTANCE hTestDll = LoadLibraryA(testDllName.c_str());
		if (hTestDll != NULL)
		{
			// Beginning of timer
			auto start = chrono::steady_clock::now();
			// Get the test function
			testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, testDllFuncName.c_str());
			if (NULL != pTestFunc)
			{
				try
				{
					// Call the test function
					bool ret = pTestFunc();

					// Build string to log
					(ret) ? snprintf(buf, MAX_BUFFER, "%d: %s %s test function returned %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "true (pass).") :
						snprintf(buf, MAX_BUFFER, "%d: %s %s test function returned %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "false (failed).");

					// Log pass / fail
					mLogger->log(Logger::INFO, logMsg, buf);
				}
				catch (int code) {
					// Build string to log
					snprintf(buf, MAX_BUFFER, "%d: %s %s %s 0x%X %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "threw exception", code, ", exception handled.");

					// Log exception
					mLogger->log(Logger::ERROR_E, logMsg, buf);
				}
				catch (...)
				{
					// Build string to log
					snprintf(buf, MAX_BUFFER, "%d: %s %s %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "test function threw a generic exception, exception handled.");

					// Log exception
					mLogger->log(Logger::ERROR_E, logMsg, buf);
				}
			}
			else
			{
				// Build string to log
				snprintf(buf, MAX_BUFFER, "%d:  %s %s %s, %s.", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "couldn't find test function", testDllFuncName);

				// Log test function error
				mLogger->log(Logger::ERROR_E, logMsg, buf);
			}

			// End the timer
			auto end = chrono::steady_clock::now();

			// Build string to log
			snprintf(buf, MAX_BUFFER, "%d: %s %s %s %d.", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "freeing dll, done. Duration (ms): ", (int)chrono::duration_cast<chrono::milliseconds>(end - start).count());

			// Free up the DLL after we're done using it, done
			mLogger->log(Logger::INFO, logMsg, buf);

			//Free the shared object
			FreeLibrary(hTestDll);

		}
		else
		{
			// Build string to log
			snprintf(buf, MAX_BUFFER, "%d: %s %s %s, %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "coudn't find dll", testDllName);

			// Log dll error
			mLogger->log(Logger::INFO, logMsg, buf);
		}
	}
}