#pragma once

#include "resource.h"
#include "constants.h"
// FileExistsDialog dialog

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
