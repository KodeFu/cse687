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

Logger::Logger()
{

}

Logger::~Logger()
{

}

void Logger::log(LOG_LEVEL level, std::string messageFormat)
{
	if (level == INFO)
	{
		std::cout << "INFO: " << messageFormat << std::endl;
	}
	else if (level == DEBUG)
	{
		std::cout << "DEBUG: " << messageFormat << std::endl;
	}
	else if (level == ERROR_E)
	{
		std::cout << "ERROR: " << messageFormat << std::endl;
	}
}