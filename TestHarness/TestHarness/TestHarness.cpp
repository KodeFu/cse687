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
#include <Windows.h>
#include <thread>
#include <mutex>
#include "TestExecutive.h"
#include "TestHarness.h"
#include "TestRequestQueue.h"
#include "LogOutputQueue.h"
#include "ChildTest.h"
#include "ThreadMessageQueue.h"

using namespace std;

std::mutex displayMutex;
// prototype for the test functions
typedef bool(__stdcall* testFunc)();

TestHarness::TestHarness()
{
	mLogMsgQueue = new ThreadMessageQueue<Message>;
	mLogger = new Logger(mLogMsgQueue);
}

class Thread_Test_obj
{
public:
	void operator() (std::string dllName, std::string testName)
	{
		std::lock_guard<std::mutex> l(displayMutex);
		{
			cout << "using thread " << std::this_thread::get_id() << " using callable with dll: " << dllName << " and test: " << testName << endl;
			test(dllName, testName);
		}
	}

	bool test(std::string testDllName, std::string testDllFuncName)
	{
		bool result;
		std::wstring wTestDllName(testDllName.begin(), testDllName.end());
		LPCWSTR lpcwstrTestDllName = wTestDllName.c_str();
		LPCSTR lpcstrTestDllFuncName;

		// get a handle to the test dll
		// displayMutex.lock();
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
			}

			FreeLibrary(hTestDll);
			return result;
		}
		else
		{
			std::cout << "TestHarness: Could Not find DLL: " << testDllName << std::endl;
		}
		// displayMutex.unlock();
	}
};


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
	/*
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
	*/
	// Grab the first message off the queue, this is a blocking call
	Message msgD = tq.Dequeue();

	// Declare string variables for holding DLL and Function Name
	std::string testDllName = msgD.filePath;
	std::string testDllFuncName = msgD.functionName;
	thread t1(Thread_Test_obj(), testDllName, testDllFuncName);

	msgD = tq.Dequeue();
	testDllName = msgD.filePath;
	testDllFuncName = msgD.functionName;
	thread t2(Thread_Test_obj(), testDllName, testDllFuncName);

	msgD = tq.Dequeue();
	testDllName = msgD.filePath;
	testDllFuncName = msgD.functionName;
	thread t3(Thread_Test_obj(), testDllName, testDllFuncName);

	// wait for em
	// t1.join();
	// t2.join();
	// t3.join();

	t1.detach();
	t2.detach();
	t3.detach();
	cout << "<return" << endl;
	cin.get();

}

