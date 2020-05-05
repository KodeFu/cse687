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
#include "ThreadMessageQueue.h"

class Logger
{
public:
	enum LOG_LEVEL
	{
		INFO,
		DEBUG,
		ERROR_E
	};

	Logger(ThreadMessageQueue<Message>* msgQueue);			// Constructor
	~Logger();												// Destructor
	void log(LOG_LEVEL level, std::string messageFormat);	// log function.  Writes to log queue.

private:
	ThreadMessageQueue<Message>* mLogMsgQueue;
};