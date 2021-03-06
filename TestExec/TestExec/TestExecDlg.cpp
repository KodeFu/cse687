//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: This file contains the 'main' function for the TestExec Dialog-based GUI.
//
// Parameters:  None
// Create Date:  4/25/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	Mudit Vats				Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "framework.h"
#include "TestExec.h"
#include "TestExecDlg.h"
#include "afxdialogex.h"
#include "TestData.h"

#include <thread>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// prototype for the test functions
typedef bool(__stdcall* testFunc)();

// message glue
#define WM_USER_LOG_MESSAGE  WM_USER + 1

// get a reference to logger
ProcessMessageQueue *g_logQueue;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CTestExecDlg dialog



CTestExecDlg::CTestExecDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTEXEC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	
}

void CTestExecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGMESSAGES, m_LoggerListBox);
	DDX_Control(pDX, IDC_AVAILABLETESTS, m_AvailableTests);
	DDX_Control(pDX, IDC_TESTQUEUE, m_TestQueue);
}

BEGIN_MESSAGE_MAP(CTestExecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_AVAILABLETESTS, &CTestExecDlg::OnLbnDblclkAvailableTests)
	ON_BN_CLICKED(IDC_BROWSE, &CTestExecDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_COPYALL, &CTestExecDlg::OnBnClickedCopyall)
	ON_BN_CLICKED(IDC_COPY, &CTestExecDlg::OnBnClickedCopy)
	ON_BN_CLICKED(IDC_RESET, &CTestExecDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_RUNTESTS, &CTestExecDlg::OnBnClickedRuntests)
	ON_MESSAGE(WM_USER_LOG_MESSAGE, OnUserDefinedMessage) // custom message to handle log PostMessages
END_MESSAGE_MAP()


// CTestExecDlg message handlers

BOOL CTestExecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	// Connect to the test queue
	testQueue.ClientConnect("127.0.0.1", 5005);

	// Connect to the log queue
	logQueue.ClientConnect("127.0.0.1", 5006);
	g_logQueue = &logQueue;

	// Start logging thread
	std::thread first(logThread);
	first.detach();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestExecDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestExecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestExecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestExecDlg::OnLbnDblclkAvailableTests()
{
	// TODO: Add your control notification handler code here
	OnBnClickedCopy();
}


void CTestExecDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	const TCHAR szFilter[] = _T("DLL Files (*.dll)|*.dll|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("dll"), NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, szFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		CString fileName = dlg.GetFileName();

		if (!fileName.IsEmpty())
		{
			// single selection
			GetFuncNamesFromDLL(filePath, fileName);
		}
		else
		{
			// multiple selection
			char* str = dlg.m_ofn.lpstrFile;
			std::string directory = str;
			str += (directory.length() + 1);
			while (*str) 
			{
				std::string filename = str;
				str += (filename.length() + 1);

				CString fullFilePath = filePath + "\\" + filename.c_str();
				GetFuncNamesFromDLL(fullFilePath, filename.c_str());
			}
		}
	}

}


void CTestExecDlg::GetFuncNamesFromDLL(CString filePath, CString fileName)
{
	// get a handle to the test dll
	HINSTANCE hTestDll = LoadLibrary(filePath);
	if (hTestDll != NULL)
	{
		for (int i = 1; i < 10; i++)
		{
			CString funcName;
			funcName.Format(_T("Test%d"), i);

			// convert CString to LPCTR
			CStringA tempCString(funcName);
			LPCSTR lpctrFuncName = tempCString;

			// get a pointer to the function
			testFunc pTestFunc = (testFunc)GetProcAddress(hTestDll, lpctrFuncName);
			if (NULL != pTestFunc)
			{
				// add the function name to the available tests
				CString tmp;
				tmp.Format(_T("%s --> %s()"), fileName, funcName);
				int currItemIndex = m_AvailableTests.AddString(tmp);

				// save test data
				TestData *testData = new TestData();
				testData->setFilePath(filePath);
				testData->setFileName(fileName);
				testData->setTestName(funcName);

				m_AvailableTests.SetItemData(currItemIndex, (DWORD_PTR) testData);
			}
			else
			{
				//MessageBox(_T("Coudn't find test function"), _T("Woops!"), MB_OK);
			}
		}
		FreeLibrary(hTestDll);
	}
	else
	{
		MessageBox(_T("Coudn't load DLL"), _T("Woops!"), MB_OK);
	}

	// set to first item
	if (m_AvailableTests.GetCount() > 0)
	{
		m_AvailableTests.SetCurSel(0);
	}
}

// copy all tests
void CTestExecDlg::OnBnClickedCopyall()
{
	if (m_AvailableTests.GetCount() > 0)
	{
		for (int i = 0; i < m_AvailableTests.GetCount(); i++)
		{
			CString tmp;
			m_AvailableTests.GetText(i, tmp);
			int testQueueItemIndex = m_TestQueue.AddString(tmp);
			m_TestQueue.SetItemData(testQueueItemIndex, m_AvailableTests.GetItemData(i));
		}
	}
}

// copy one test
void CTestExecDlg::OnBnClickedCopy()
{
	if (m_AvailableTests.GetCount() > 0)
	{
		int selectedItem = m_AvailableTests.GetCurSel();
		if (selectedItem >= 0)
		{
			CString tmp;
			m_AvailableTests.GetText(selectedItem, tmp);
			int testQueueItemIndex = m_TestQueue.AddString(tmp);
			m_TestQueue.SetItemData(testQueueItemIndex, m_AvailableTests.GetItemData(selectedItem));
		}
	}
}

// reset availalbe tests and test queue
void CTestExecDlg::OnBnClickedReset()
{
	m_AvailableTests.ResetContent();
	m_TestQueue.ResetContent();
}

// run tests
void CTestExecDlg::OnBnClickedRuntests()
{
	if (m_TestQueue.GetCount() > 0)
	{
		for (int i = 0; i < m_TestQueue.GetCount(); i++)
		{
			TestData *testData = (TestData *) m_TestQueue.GetItemData(i);
			if (testData)
			{
				CString filePath = testData->getFilePath();
				CString fileName = testData->getFileName();
				CString testName = testData->getTestName();
				
				// enqueue test using filePath, fileName, testName
				//m_LoggerListBox.AddString(filePath);
				//m_LoggerListBox.AddString(fileName);
				//m_LoggerListBox.AddString(testName);

				// queue it up for the TestHarness
				Message msg;
				msg.filePath = filePath.GetString();
				msg.functionName = testName.GetString();
				testQueue.Enqueue(msg);

			}
		}
	}
}

// thread which polls for new log messages in the log queue and
// if available, sends them to the log message handler
void CTestExecDlg::logThread()
{
	CString* b = new CString("Hello Logger!");
	CWnd* pMainWnd = AfxGetApp()->GetMainWnd();
	if (pMainWnd)
	{
		pMainWnd->PostMessage(WM_USER_LOG_MESSAGE, NULL, (LPARAM)b);
	}

	// get log messages from the log queue from the Test Harness
	while (true)
	{
		if (!g_logQueue->isEmpty())
		{
			Message msg = g_logQueue->Dequeue();

			if (pMainWnd)
			{
				CString *b = new CString(msg.message.c_str());
				pMainWnd->PostMessage(WM_USER_LOG_MESSAGE, NULL, (LPARAM)b);
			}

			Sleep(500);
		}
	}

}

// log message handler to show log message
afx_msg LRESULT CTestExecDlg::OnUserDefinedMessage(WPARAM wParam, LPARAM lParam)
{
	CString *cslParam = (CString *) lParam; 
	CString friendlyMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	if (cslParam)
	{
		CTime t = CTime::GetCurrentTime();

		CString formattedString;
		formattedString.Format("%s %02d %02d:%02d:%02d %s", friendlyMonth[t.GetMonth()-1], t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond(), *cslParam);
		m_LoggerListBox.AddString(formattedString);
		delete cslParam;
	}

	return 0;
}