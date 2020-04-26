#include "pch.h"
#include <iostream>
#include "TestFuncsA.h"

// Test1: just returns true
bool Test1()
{
	std::cout << "TestFuncsA.dll: Hello from Test1 function." << std::endl;
	return true;
}

// Test2: just returns false
bool Test2()
{
	std::cout << "TestFuncsA.dll: Hello from Test2 function." << std::endl;
	return false;
}

// Test3: throws and exception
bool Test3()
{
	std::cout << "TestFuncsA.dll: Hello from Test3 function." << std::endl;

	throw 0xdead;

	return true;
}

// Test4: takes some time; 5 seconds
bool Test4()
{
	std::cout << "TestFuncsA.dll: Hello from Test4 function." << std::endl;

	Sleep(1000 * 5);

	return true;
}


// Test5: takes some time; 15 seconds
bool Test5()
{
	std::cout << "TestFuncsA.dll: Hello from Test5 function." << std::endl;

	Sleep(1000 * 15);

	return true;
}

// Test6: takes some time; 10 seconds; throws exception
bool Test6()
{
	std::cout << "TestFuncsA.dll: Hello from Test6 function." << std::endl;

	Sleep(1000 * 10);

	throw 0xdead;

	return true;
}


// Test4: takes some time; 20 seconds; throws exception
bool Test7()
{
	std::cout << "TestFuncsA.dll: Hello from Test7 function." << std::endl;

	Sleep(1000 * 20);

	throw 0xdead;

	return true;
}