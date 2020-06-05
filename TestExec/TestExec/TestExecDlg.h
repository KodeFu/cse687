//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Author:  Archer_Howick_Pretola_Vats
// CSE687_M400_Project01.cpp : Object Oriented Design
// Syracuse University
// Project
// Requirements: This file contains the header file for the TestExec Dialog-based GUI.
//
// Parameters:  None
// Create Date:  4/25/2020
// Revision History:
// Date			Programmer/Developer	Reason
// 4/22/2020	Mudit Vats				Original
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "ProcessMessageQueue.h"

// CTestExecDlg dialog
class CTestExecDlg : public CDialogEx
{
// Construction
public:
	CTestExecDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTEXEC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_LoggerListBox;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnLbnDblclkAvailableTests();
	CListBox m_AvailableTests;
	CListBox m_TestQueue;
	afx_msg void OnBnClickedBrowse();

private:
	void GetFuncNamesFromDLL(CString filePath, CString fileName);
public:
	afx_msg void OnBnClickedCopyall();
	afx_msg void OnBnClickedCopy();
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRuntests();

private:
	static void logThread();
	ProcessMessageQueue testQueue;
	ProcessMessageQueue logQueue;

public:
	afx_msg LRESULT OnUserDefinedMessage(WPARAM wParam, LPARAM lParam);
};
