#pragma once

#include "resource.h"
#include "afxwin.h"

// CCalcGap dialog

class CCalcGap : public CDialog
{
	DECLARE_DYNAMIC(CCalcGap)

public:
	CCalcGap(CString FilePath, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCalcGap();
	CString m_MeasFile;
	CString m_AgLitFile;
	CString	m_C0;
	CString m_b;
	CString m_ro;
	CString m_ri;
	CString m_AgZero;
	CString m_TCOL;
	CString m_CCOL;
	CString m_OutputFile;
	CString m_Output;

	afx_msg void OnBnClickedBtnMeasfile();
	afx_msg void OnBnClickedBtnAglit();
	afx_msg void OnBnClickedBtnCalculategap();
	afx_msg void OnBnClickedBtnOutputfile();
	afx_msg void OnBnClickedBtnHelp2();
	afx_msg void OnBnClickedBtnAgzero();

	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_CalcGap };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	
private:
	void CalculateGap();
	
};


