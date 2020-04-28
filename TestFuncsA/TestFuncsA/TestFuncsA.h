#pragma once

#ifdef TESTFUNCSA_EXPORTS
#define TESTFUNCSA_API __declspec(dllexport)
#else
#define TESTFUNCSA_API __declspec(dllimport)
#endif

// Test1
extern "C" TESTFUNCSA_API bool Test1();

// Test2
extern "C" TESTFUNCSA_API bool Test2();

// Test3
extern "C" TESTFUNCSA_API bool Test3();

// Test4
extern "C" TESTFUNCSA_API bool Test4();

// Test5
extern "C" TESTFUNCSA_API bool Test5();

// Test6
extern "C" TESTFUNCSA_API bool Test6();

// Test7
extern "C" TESTFUNCSA_API bool Test7();