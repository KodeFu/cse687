//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// Logger.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: See file Archer_Howick_Pretola_Vats_CSE687_Project1_ArchitectureDocx002.pdf for complete
//		project requirements, architecture, and design.
// Parameters:  None
// Create Date:  4/25/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/25/2020	Troy H Archer			Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Logger.h"
#include <iostream>

#define MAX_SIZE_MESSAGE_SHORT		90     // used for sprintfs

Logger::Logger(ProcessMessageQueue* msgQueue) :
	mLogMsgQueue(msgQueue)
{

}

Logger::~Logger()
{

}

void Logger::log(LOG_LEVEL level, Message msg, std::string messageFormat)
{
	// Convert the enum into a std::string
	std::string enumString = convertEnumToString(level);

	// Get the length of the message so far
	int msgLength = messageFormat.length();

	//msg.message = "Test...";
	
	// If the message is less than the max buffer
	if (msgLength < MAX_SIZE_MESSAGE_SHORT)
	{
		// If the message length and enum length are greater than the MAX_BUFFER
		if (enumString.length() + msgLength >= MAX_SIZE_MESSAGE_SHORT)
		{
			// Don't include the enum if it overflows the buffer
			msg.message = messageFormat;
		}
		else
		{
			// Include the enum if it does NOT overflow the buffer
			msg.message = enumString + messageFormat;
		}
	}
	else
	{
		// Handle the case of overflowing the buffer
		msg.message = convertEnumToString(ERROR_E) + "Error processing message, names too long!";
	}

	//Enqueue the message
	mLogMsgQueue->Enqueue(msg);
}

std::string Logger::convertEnumToString(enum LOG_LEVEL level)
{
	if (level == INFO)
	{
		return "INFO: ";
	}
	else if (level == DEBUG)
	{
		return "DEBUG: ";
	}
	else if (level == ERROR_E)
	{
		return  "ERROR: ";
	}
}