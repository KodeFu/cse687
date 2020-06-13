//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// Logger.h : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/25/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	David L Howick			Original
// 4/23/2020	Troy H Archer			Adding initial content
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include "ProcessMessageQueue.h"

class Logger
{
public:
	enum LOG_LEVEL
	{
		INFO,
		DEBUG,
		ERROR_E
	};
	//log message for a duration based on the time ex. Test 7 lasted 7 seconds! message for example
	Logger(ProcessMessageQueue* msgQueue);			// Constructor
	~Logger();												// Destructor
	void log(LOG_LEVEL level, Message msg, std::string messageFormat);	// log function.  Writes to log queue.
	std::string convertEnumToString(enum LOG_LEVEL level); // Convert the enum value into a string value

private:
	ProcessMessageQueue* mLogMsgQueue;
};