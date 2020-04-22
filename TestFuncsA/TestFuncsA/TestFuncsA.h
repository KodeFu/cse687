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
