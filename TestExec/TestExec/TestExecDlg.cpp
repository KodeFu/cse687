
// TestExecDlg.cpp : implementation file
//

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
	ON_MESSAGE(WM_USER_LOG_MESSAGE, OnUserDefinedMessage)
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
			wchar_t* str = dlg.m_ofn.lpstrFile;
			std::wstring directory = str;
			str += (directory.length() + 1);
			while (*str) 
			{
				std::wstring filename = str;
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


void CTestExecDlg::OnBnClickedReset()
{
	m_AvailableTests.ResetContent();
	m_TestQueue.ResetContent();
}


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
				// ...
				m_LoggerListBox.AddString(filePath);
				m_LoggerListBox.AddString(fileName);
				m_LoggerListBox.AddString(testName);
			}
		}
	}
}

void CTestExecDlg::logThread()
{
	int a = 10;
	
	CString *b = new CString("Hello, here's a log message!");
	CWnd *pMainWnd = AfxGetApp()->GetMainWnd();
	if (pMainWnd)
	{
		pMainWnd->PostMessageW(WM_USER_LOG_MESSAGE, a, (LPARAM) b);
	}

	// TBD: while loop to check for new messages, if there is one, 
	// dequeue and then PostMessage for the main thread to picku it
	// up.
}

afx_msg LRESULT CTestExecDlg::OnUserDefinedMessage(WPARAM wParam, LPARAM lParam)
{
	CString *cslParam = (CString *) lParam; 
	m_LoggerListBox.AddString(cslParam->GetString());

	return 0;
}