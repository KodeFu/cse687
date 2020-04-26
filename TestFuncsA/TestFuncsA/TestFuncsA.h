#pragma once

#ifdef TESTFUNCSA_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif

// Test1
extern "C" MATHLIBRARY_API bool Test1();

// Test2
extern "C" MATHLIBRARY_API bool Test2();

// Test3
extern "C" MATHLIBRARY_API bool Test3();

// Test4
extern "C" MATHLIBRARY_API bool Test4();

// Test5
extern "C" MATHLIBRARY_API bool Test5();

// Test6
extern "C" MATHLIBRARY_API bool Test6();

// Test7
extern "C" MATHLIBRARY_API bool Test7();