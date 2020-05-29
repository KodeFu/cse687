# Test Harness Project
Repository for CSE687 Project. This repository consists of the following components:
- TestHarness
    - Main Test Harness executable
- TestFuncsA
    - Test DLL
- TestExec
    - GUI for project 3+ 

# Requirements
- Visual Studio 2019
- Use Windows Add/Remove programs to Modify Visual 2019 components.
- Add "Desktop development with C++".
    - Selectable in the Workloads tab.
- Add "C++ MFC for latest v142 build tools (x86 & x64)".
    - Selectable in the Individual Components tab.
    - This is required to build TestExec (GUI) which depends on  Microsoft Foudation Classes (MFC).

# Build
TestHarness is the main test execution engine which will open test dlls, execute tests and report results. TestExec is the GUI / user interface which allows the use to select which tests to run and shows test execution log messages.

To Build:
- Open TestHarness\TestHarness.sln
    - Build the TestTHarness solution which will build the following projects:
        - TestFuncsA Project
        - TestHarness Project
    - After the build completes, the TestFuncsA.dll, TestFuncsB.dll, and TestFuncsC.dll will be automatically copied to the same location as the TestHarness.exe.
- Open TestExec\TestExec.sln
    - Build the TestExec solution.

# Execution
Since TestHarness acts as a server for the test queue and logging queue, it should be run first. This then allows the TestExec to make connections to TestHarness to enqueue tests and receive log messages.

1.  Run TestHarness from the Visual Studio IDE.
2.  Run TestExec from the Visual Studio IDE.

** Alternatively, run TestHarness.exe from the TestHarness\x64\Debug directory and TestExec.exe from the TestExec\x64\Debug directory.

From TestExec, browse to a test dll (i.e. TestFuncsA.dll in the TestHarness\x64\Debug directory). Select the tests which you'd like to run - they will be moved to the test queue. Click on Run Tests.

TestHarness runs as a console application and executes tests.
![TestHarness](https://github.com/KodeFu/cse687/blob/master/Archive/TestHarness.png)


TestExec runs as a GUIL application and interacts with the user to select tests to execute and show log messages.
![TestExec](https://github.com/KodeFu/cse687/blob/master/Archive/TestExec.png)

