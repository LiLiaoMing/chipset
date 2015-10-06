// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "InputDlg.h"


// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{
	m_strName = _T("");
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
	ON_EN_CHANGE(IDC_NAME_EDIT, &CInputDlg::OnEnChangeNameEdit)
END_MESSAGE_MAP()


// CInputDlg message handlers

void CInputDlg::OnEnChangeNameEdit()
{
	GetDlgItemText(IDC_NAME_EDIT, m_strName);
}
