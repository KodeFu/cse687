#pragma once
#include "pch.h"

class TestData
{
public:
	TestData();
	~TestData();

	void setFilePath(CString filePath);
	void setFileName(CString fileName);
	void setTestName(CString testName);

	CString getFilePath();
	CString getFileName();
	CString getTestName();

private:
	CString _filePath;
	CString _fileName;
	CString _testName;
};

