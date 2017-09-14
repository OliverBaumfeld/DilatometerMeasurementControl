// FileExistsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FileExistsDialog.h"
#include "afxdialogex.h"


// FileExistsDialog dialog

IMPLEMENT_DYNAMIC(FileExistsDialog, CDialog)

FileExistsDialog::FileExistsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(FileExistsDialog::IDD, pParent)
{

}

FileExistsDialog::~FileExistsDialog()
{
}

void FileExistsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FileExistsDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &FileExistsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_APPEND, &FileExistsDialog::OnBnClickedAppend)
	ON_BN_CLICKED(IDC_OVERWRITE, &FileExistsDialog::OnBnClickedOverwrite)
END_MESSAGE_MAP()

// FileExistsDialog message handlers


void FileExistsDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void FileExistsDialog::OnBnClickedAppend()
{
	// TODO: Add your control notification handler code here
	EndDialog(OUTPUT_APPEND);
}


void FileExistsDialog::OnBnClickedOverwrite()
{
	// TODO: Add your control notification handler code here
	EndDialog(OUTPUT_OVERWRITE);
}
