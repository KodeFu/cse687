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
#include "TestHarness.h"
#include "ThreadMessageQueue.h"

using namespace std;

#define MAX_BUFFER			256     // used for sprintfs
#define THREAD_POOL_MAX		5		// number of threads in the thread pool

// childTester function
void childTester(ProcessMessageQueue& testQueue, ProcessMessageQueue& logQueue);

// prototype for the test functions
typedef bool(__stdcall* testFunc)();


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
	//Logger* log = new Logger(logQueue);

	for (int i = 0; i < THREAD_POOL_MAX; i++)
	{
		std::thread{ childTester, std::ref(testQueue), std::ref(logQueue) }.detach();
	}
	
	// wait for key press
	cin.get();

}

void childTester(ProcessMessageQueue& testQueue, ProcessMessageQueue& logQueue)
{
	Message log;
	char buf[MAX_BUFFER];;

	// show starting thread and ids
	cout << GetCurrentThreadId() << ": started thread" << endl;

	// loop forever
	while (true)
	{
		// check if there is a new test to execute
		if (testQueue.isEmpty())
		{
			Sleep(1000);
			continue;
		}

		// dequeue the test
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
		
		snprintf(buf, MAX_BUFFER, "%d: %s %s %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "start test");
		log.message = buf;
		logQueue.Enqueue(log);

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
					bool ret = pTestFunc();

					// log pass / fail
					(ret) ? snprintf(buf, MAX_BUFFER, "%d: %s %s test function returned %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "true (pass)") :
						snprintf(buf, MAX_BUFFER, "%d: %s %s test function returned %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "false (failed)");

					log.message = buf;
					logQueue.Enqueue(log);
				}
				catch (int code) {
					// log exception
					snprintf(buf, MAX_BUFFER, "%d: %s %s %s 0x%X %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "test function threw exception", code, ", exception handled");
					log.message = buf;
					logQueue.Enqueue(log);
				}
				catch (...)
				{
					// log exception
					snprintf(buf, MAX_BUFFER, "%d: %s %s %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "test function threw a generic exception, exception handled");
					log.message = buf;
					logQueue.Enqueue(log);
				}
			}
			else
			{
				// log test function error
				snprintf(buf, MAX_BUFFER, "%d:  %s %s %s, %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "couldn't find test function", testDllFuncName);
				log.message = buf;
				logQueue.Enqueue(log);
			}

			// free up the DLL after we're done using it, done
			snprintf(buf, MAX_BUFFER, "%d: %s %s %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "freeing dll, done");
			log.message = buf;
			logQueue.Enqueue(log);

			FreeLibrary(hTestDll);
			
		}
		else
		{
			// log dll error
			snprintf(buf, MAX_BUFFER, "%d: %s %s %s, %s", GetCurrentThreadId(), testDllShortName, testDllFuncName.c_str(), "coudn't find dll", testDllName);
			log.message = buf;
			logQueue.Enqueue(log);
		}
	}
}