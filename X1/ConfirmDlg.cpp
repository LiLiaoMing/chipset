// ConfirmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "ConfirmDlg.h"


// CConfirmDlg dialog

IMPLEMENT_DYNAMIC(CConfirmDlg, CDialog)

CConfirmDlg::CConfirmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfirmDlg::IDD, pParent)
{

}

CConfirmDlg::~CConfirmDlg()
{
}

void CConfirmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfirmDlg, CDialog)
END_MESSAGE_MAP()


// CConfirmDlg message handlers
