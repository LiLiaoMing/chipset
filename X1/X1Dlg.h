
// X1Dlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "ColorStatic.h"

class CInternalWnd;
class CIstGenWnd;
class CIstLoadWnd;
class CDBManWnd;

// CX1Dlg dialog
class CX1Dlg : public CDialog
{
// Construction
public:
	CX1Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_X1_DIALOG };

	CColorStatic m_ctlModeTitle;

	CInternalWnd*	m_pRM5Wnd;
	CIstGenWnd*		m_pIstGenWnd;
	CIstLoadWnd*	m_pIstLoadWnd;
	CDBManWnd*		m_pDBManWnd;

	CxImage			m_img;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:

	int		m_nMode;
	CRect	m_rcMin;
	
	void SetStaticString(UINT uid, int fntsize, char* tip, BOOL bInit=FALSE);

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
	afx_msg void OnFileExit();
	afx_msg void OnHelpAbout();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnModeInternal();
	afx_msg void OnUpdateModeInternal(CCmdUI *pCmdUI);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnModeIstgenerate();
	afx_msg void OnUpdateModeIstgenerate(CCmdUI *pCmdUI);
	afx_msg void OnModeIstload();
	afx_msg void OnUpdateModeIstload(CCmdUI *pCmdUI);
	afx_msg void OnModeManagedb();
	afx_msg void OnUpdateModeManagedb(CCmdUI *pCmdUI);
};
