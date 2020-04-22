// TestFuncsTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>

// prototype for the test functions
typedef bool(__stdcall* testFunc)();

int main()
{
    // get a handle to the test dll
    HINSTANCE hTestDll = LoadLibrary(L"TestFuncsA.DLL");
    if (hTestDll != NULL)
    {
        // get a pointer to the function
        testFunc pTestFunc = (testFunc) GetProcAddress(hTestDll, "Test1");
        if (NULL != pTestFunc)
        {
            // run the function
            bool result = pTestFunc();
            std::cout << "test function returned: " << result << std::endl;
        }
        else
        {
            std::cout << "couldn't find test function" << std::endl;
        }

        FreeLibrary(hTestDll);
    }
    else
    {
        std::cout << "coudn't find dll" << std::endl;
    }
}
