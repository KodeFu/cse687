#include "pch.h"
#include "TestData.h"

TestData::TestData()
	: _filePath(""), _fileName(""), _testName("")
{

}

TestData::~TestData()
{

}

void TestData::setFilePath(CString filePath)
{
	_filePath = filePath;
}

void TestData::setFileName(CString fileName)
{
	_fileName = fileName;
}

void TestData::setTestName(CString testName)
{
	_testName = testName;
}

CString TestData::getFilePath()
{
	return _filePath;
}

CString TestData::getFileName()
{
	return _fileName;
}

CString TestData::getTestName()
{
	return _testName;
}