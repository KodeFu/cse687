
// TestExecDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "TestExec.h"
#include "TestExecDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// prototype for the test functions
typedef bool(__stdcall* testFunc)();

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
	afx_msg void OnLbnSelchangeList1();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAboutDlg::OnLbnSelchangeList1)
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
	DDX_Control(pDX, IDC_LIST1, m_LoggerListBox);
	DDX_Control(pDX, IDC_LIST4, m_AvailableTests);
	DDX_Control(pDX, IDC_LIST3, m_TestQueue);
}

BEGIN_MESSAGE_MAP(CTestExecDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LIST1, &CTestExecDlg::OnLbnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestExecDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BROWSE, &CTestExecDlg::OnBnClickedBrowse)
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



void CAboutDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("Hello"), _T("Hello2"), MB_OK);
}


void CTestExecDlg::OnLbnDblclkList1()
{
	// TODO: Add your control notification handler code here
	int currSel = m_LoggerListBox.GetCurSel();
	CString blah;
	blah.Format(_T("Current Selection: %d"), currSel);
	MessageBox(blah, L"Yo", MB_OK);
}


void CTestExecDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here
	m_LoggerListBox.AddString(_T("TestDLL1, Test A passed"));
	m_LoggerListBox.AddString(_T("TestDLL1, Test C passed"));
	m_LoggerListBox.AddString(_T("TestDLL1, Test Z failed"));
	m_LoggerListBox.AddString(_T("TestDLL1, Test B passed"));

	m_TestQueue.AddString(_T("TestDLL1, Test A"));
	m_TestQueue.AddString(_T("TestDLL3, Test ZZZZ"));
	m_TestQueue.AddString(_T("TestDLL2, Test CB"));
	m_TestQueue.AddString(_T("TestDLL4, Test Zzz"));
	m_TestQueue.AddString(_T("TestDLL2, Test Bob"));

	m_AvailableTests.AddString(_T("TestDLL1, Test A"));
	m_AvailableTests.AddString(_T("TestDLL3, Test ZZZZ"));
	m_AvailableTests.AddString(_T("TestDLL2, Test CB"));
	m_AvailableTests.AddString(_T("TestDLL4, Test Zzz"));
	m_AvailableTests.AddString(_T("TestDLL2, Test Bob"));
}


void CTestExecDlg::OnBnClickedBrowse()
{
	// TODO: Add your control notification handler code here
	const TCHAR szFilter[] = _T("DLL Files (*.dll)|*.dll|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("dll"), NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, szFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		// do something
		GetFuncNamesFromDLL(sFilePath);
	}

}


void CTestExecDlg::GetFuncNamesFromDLL(CString filePath)
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
				m_AvailableTests.AddString(funcName);
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
}