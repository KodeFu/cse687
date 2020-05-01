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

#pragma once
#include <iostream>
#include <string>

class TestRequestQueue
{
public:
	TestRequestQueue();										// Constructor
	~TestRequestQueue();									// Destructor
	void enqueueDll(std::string);							// enqueue function
	std::string dequeueDll();								// dequeue function
	void enqueueTest(std::string);							// enqueue function
	std::string dequeueTest();								// dequeue function

private:
	std::string dllqueue;
	std::string testqueue;
};
