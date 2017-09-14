/*==========================================================================
 *                   PPMSV0.1Dlg.h : header file
 *
 *                 contains the class definitions
 *==========================================================================
 */
#pragma once

#include <string>
#include <vector>
#include "afxwin.h"

/*===========================================================================
 *
 *                CPPMSDialog dialog, the main dialog
 *
 *===========================================================================
 */
class CPPMSDialog : public CDialogEx
{
// Construction
public:
	CPPMSDialog(CWnd* pParent = NULL);	// standard constructor

	enum { IDD = IDD_PPMSV01_DIALOG };

	CString m_DisplayStatus;
	
	afx_msg void OnBnClickedSavefileBtn();
	CString m_SaveFile;
	afx_msg void OnBnClickedHelpBtn();
	virtual void OnOK();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	void ErrorHandler( int intErrorID );
	std::string ReturnGPIBError();
	void ProcessAdvisory(UINT message, WPARAM wParam, LPARAM lParam);
	HANDLE m_hAdvisory;
	int OutPutMode;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	int nReceivedAdvisories;
	void CalculateGap();
	std::vector<double> vCernox;
	std::vector<double> a0, a1, a2, a3, vR;  // for interpolation of the Cernox temperature
	bool bCernoxLoaded;
public:
	afx_msg void OnBnClickedBtnCalculations();
	CButton m_CernoxSensor;
};


/*=========================================================
 *                    Help dialog
 *=========================================================
 */
class CHelpDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CHelpDialog)

public:
	CHelpDialog(CString strHelpText, CWnd* pParent = NULL);   // standard constructor
	virtual ~CHelpDialog();

// Dialog Data
	enum { IDD = IDD_HELPDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_HelpText;
	virtual BOOL OnInitDialog();
	CEdit m_HelpCEdit;
	CFont m_font;
};


/*===========================================================================
 *                       FileExistsDialog dialog
 *===========================================================================
 */
class FileExistsDialog : public CDialog
{
	DECLARE_DYNAMIC(FileExistsDialog)

public:
	FileExistsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~FileExistsDialog();

// Dialog Data
	enum { IDD = IDD_FILEEXISTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAppend();
	afx_msg void OnBnClickedOverwrite();
};


/*===========================================================================
 *                CAboutDlg dialog  (unused)
 *===========================================================================
 */
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	DECLARE_MESSAGE_MAP()
};


/*======================================
 *         global functions 
 *======================================*/
