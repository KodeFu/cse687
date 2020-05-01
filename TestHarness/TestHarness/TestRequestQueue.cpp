//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/22/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	David L Howick			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "TestRequestQueue.h"


// Constructor
TestRequestQueue::TestRequestQueue()
{
	enqueueDll("TestFuncsA.DLL");
	enqueueTest("Test1");
}

// Destructor
TestRequestQueue::~TestRequestQueue() {}

void TestRequestQueue::enqueueDll(std::string DllName)
{
	dllqueue = DllName;
}

std::string TestRequestQueue::dequeueDll()
{
	return dllqueue;
}

void TestRequestQueue::enqueueTest(std::string testName)
{
	testqueue = testName;
}

std::string TestRequestQueue::dequeueTest()
{
	return testqueue;
}