#include "pch.h"
#include <iostream>
#include "TestFuncsA.h"

// Test1
bool Test1()
{
	std::cout << "TestFuncsA.dll: Hello from Test1 function." << std::endl;
	return true;
}

// Test2
bool Test2()
{
	std::cout << "TestFuncsA.dll: Hello from Test2 function." << std::endl;
	return false;
}

// Test3
bool Test3()
{
	std::cout << "TestFuncsA.dll: Hello from Test3 function." << std::endl;
	return true;
}