
// X1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "X1Dlg.h"

#include "InternalWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CX1Dlg dialog




CX1Dlg::CX1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CX1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pRM5Wnd = NULL;
	m_pIstGenWnd = NULL;
	m_pIstLoadWnd = NULL;
	m_pDBManWnd = NULL;
	m_nMode = -1;
}

void CX1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_ctlModeTitle);
}

BEGIN_MESSAGE_MAP(CX1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_EXIT, &CX1Dlg::OnFileExit)
	ON_COMMAND(ID_HELP_ABOUT, &CX1Dlg::OnHelpAbout)
	ON_WM_INITMENUPOPUP()
	ON_COMMAND(ID_MODE_INTERNAL, &CX1Dlg::OnModeInternal)
	ON_UPDATE_COMMAND_UI(ID_MODE_INTERNAL, &CX1Dlg::OnUpdateModeInternal)
	ON_COMMAND(ID_MODE_ISTGENERATE, &CX1Dlg::OnModeIstgenerate)
	ON_UPDATE_COMMAND_UI(ID_MODE_ISTGENERATE, &CX1Dlg::OnUpdateModeIstgenerate)
	ON_COMMAND(ID_MODE_ISTLOAD, &CX1Dlg::OnModeIstload)
	ON_UPDATE_COMMAND_UI(ID_MODE_ISTLOAD, &CX1Dlg::OnUpdateModeIstload)
	ON_COMMAND(ID_MODE_MANAGEDB, &CX1Dlg::OnModeManagedb)
	ON_UPDATE_COMMAND_UI(ID_MODE_MANAGEDB, &CX1Dlg::OnUpdateModeManagedb)
END_MESSAGE_MAP()


// CX1Dlg message handlers
void GetDlgItemRect(HWND hWnd, UINT id, CRect& rcItem)
{
	WINDOWPLACEMENT pl;
	HWND pWnd = GetDlgItem(hWnd, id);

	::GetWindowPlacement(pWnd, &pl);
	rcItem = pl.rcNormalPosition;
}

void CX1Dlg::SetStaticString(UINT uid, int fntsize, char* tip, BOOL bInit)
{
	if(bInit) {
		m_ctlModeTitle.SubclassDlgItem(uid, this);
		m_ctlModeTitle.SetFontInfo(_T("Microsoft Sans Serif"), fntsize, TRUE);
		m_ctlModeTitle.SetBackColor(RGB(222, 223, 222));
		m_ctlModeTitle.SetTextColor(RGB(78,130,55/*153, 180, 209*/));
	}
	m_ctlModeTitle.SetCaption(tip);

	//Remove title region.
	CWnd*pWnd = GetDlgItem(IDC_STATIC_TITLE);
	CRect rcTtile;

	pWnd->GetClientRect(rcTtile);
	InvalidateRect(rcTtile);

	// Redraw title region
	m_ctlModeTitle.Invalidate();
}

BOOL CX1Dlg::OnInitDialog()
{
	SetStaticString(IDC_STATIC_TITLE, 32, _T("SmartCard AllInOne"), TRUE);

	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	WINDOWPLACEMENT pl;
	GetWindowPlacement(&pl);
	m_rcMin = pl.rcNormalPosition;
	//GetAllDlgItemsRegion();

	CRect rcITem;
	m_pRM5Wnd = new CInternalWnd(this);
	m_pRM5Wnd->Create( IDD_INTERNAL_DIALOG, this );
	GetDlgItemRect(m_hWnd, IDC_STATIC_AREA, rcITem);
	m_pRM5Wnd->SetWindowPos(&wndTop, rcITem.left, rcITem.top, rcITem.Width(),rcITem.Height(),SWP_HIDEWINDOW );

	/*
	m_pIstGenWnd = new CIstGenWnd(this);
	m_pIstGenWnd->Create( IDD_IST_GEN_DIALOG, this );
	m_pIstGenWnd->SetWindowPos(&wndTop, rcITem.left, rcITem.top, rcITem.Width(),rcITem.Height(),SWP_HIDEWINDOW );

	m_pIstLoadWnd = new CIstLoadWnd(this);
	m_pIstLoadWnd->Create( IDD_IST_LOAD_DIALOG, this );
	m_pIstLoadWnd->SetWindowPos(&wndTop, rcITem.left, rcITem.top, rcITem.Width(),rcITem.Height(),SWP_HIDEWINDOW );

	m_pDBManWnd = new CDBManWnd(this);
	m_pDBManWnd->Create( IDD_DBMAN_DIALOG, this );
	m_pDBManWnd->SetWindowPos(&wndTop, rcITem.left, rcITem.top, rcITem.Width(),rcITem.Height(),SWP_HIDEWINDOW );
	*/
	
	HRSRC handle = FindResource(NULL,MAKEINTRESOURCE(IDB_READER_BITMAP),RT_BITMAP);
	//LoadResource(FindResource(NULL,_T("IDR_JPG2"),_T("JPG")),CXIMAGE_FORMAT_JPG))
	//LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_ICO1),_T("ICO")),CXIMAGE_FORMAT_ICO))
	//LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_TIF1),_T("TIF")),CXIMAGE_FORMAT_TIF))
	//LoadResource(FindResource(NULL,MAKEINTRESOURCE(IDR_GIF1),_T("GIF")),CXIMAGE_FORMAT_GIF))
	//HRSRC handle = FindResource(NULL,MAKEINTRESOURCE(IDB_READER_PNG),_T("PNG"));
	m_img.LoadResource(handle,CXIMAGE_FORMAT_BMP);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CX1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CX1Dlg::OnPaint()
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
		if(m_nMode==-1) {
			
			CPaintDC dc(this);
			CRect rcWork;
			//CWnd* pWnd = GetDlgItem(IDC_STATIC_AREA);

			//pWnd->GetClientRect(rcWork);
			GetDlgItemRect(this->m_hWnd, IDC_STATIC_AREA, rcWork);

			dc.SetBkMode(TRANSPARENT);
			m_img.Draw(dc.m_hDC, rcWork);

		}

		CDialog::OnPaint();

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CX1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CX1Dlg::OnFileExit()
{
	OnOK();
}

void CX1Dlg::OnHelpAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

BOOL CX1Dlg::DestroyWindow()
{
	if(m_pRM5Wnd) {
		m_pRM5Wnd->DestroyWindow();
		delete m_pRM5Wnd;
	}
	/*
	if(m_pIstGenWnd) {
		m_pIstGenWnd->DestroyWindow();
		delete m_pIstGenWnd;
	}

	if(m_pIstLoadWnd) {
		m_pIstLoadWnd->DestroyWindow();
		delete m_pIstLoadWnd;
	}

	if(m_pDBManWnd) {
		m_pDBManWnd->DestroyWindow();
		delete m_pDBManWnd;
	}
	*/
	return CDialog::DestroyWindow();
}

BOOL CX1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{

		//int idCtrl = this->GetFocus()->GetDlgCtrlID();
		//if (( idCtrl == IDC_TXTPIN_GSMSIM ) || ( idCtrl == IDC_TXTPIN_CHIPKNIP ))
		return true;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CX1Dlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu; // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue; // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE); // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}

void CX1Dlg::OnModeInternal()
{
	m_nMode = 1;
	SetStaticString(IDC_STATIC_TITLE,  32, _T("SmartCard AllInOne - Internal Mode"));

	m_pRM5Wnd->ShowWindow(SW_SHOW);
	/*
	m_pIstGenWnd->ShowWindow(SW_HIDE);
	m_pIstLoadWnd->ShowWindow(SW_HIDE);
	m_pDBManWnd->ShowWindow(SW_HIDE);
	*/
}

void CX1Dlg::OnModeIstgenerate()
{
	m_nMode = 2;
	SetStaticString(IDC_STATIC_TITLE,  32, _T("SmartCard AllInOne - Ist Generation Mode"));
	
	/*
	m_pRM5Wnd->ShowWindow(SW_HIDE);
	m_pIstGenWnd->ShowWindow(SW_SHOW);
	m_pIstLoadWnd->ShowWindow(SW_HIDE);
	m_pDBManWnd->ShowWindow(SW_HIDE);
	*/
}

void CX1Dlg::OnModeIstload()
{
	m_nMode = 3;
	SetStaticString(IDC_STATIC_TITLE,  32, _T("SmartCard AllInOne - Ist Load Mode"));
	/*
	m_pRM5Wnd->ShowWindow(SW_HIDE);
	m_pIstGenWnd->ShowWindow(SW_HIDE);
	m_pIstLoadWnd->ShowWindow(SW_SHOW);
	m_pDBManWnd->ShowWindow(SW_HIDE);
	*/
}

void CX1Dlg::OnModeManagedb()
{
	m_nMode = 4;
	SetStaticString(IDC_STATIC_TITLE,  32, _T("SmartCard AllInOne - Manage DB Mode"));
	/*
	m_pRM5Wnd->ShowWindow(SW_HIDE);
	m_pIstGenWnd->ShowWindow(SW_HIDE);
	m_pIstLoadWnd->ShowWindow(SW_HIDE);
	m_pDBManWnd->ShowWindow(SW_SHOW);
	*/
}

void CX1Dlg::OnUpdateModeInternal(CCmdUI *pCmdUI)
{
	if(m_nMode!=1)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}

void CX1Dlg::OnUpdateModeIstgenerate(CCmdUI *pCmdUI)
{
	if(m_nMode!=2)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}

void CX1Dlg::OnUpdateModeIstload(CCmdUI *pCmdUI)
{
	if(m_nMode!=3)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}

void CX1Dlg::OnUpdateModeManagedb(CCmdUI *pCmdUI)
{
	if(m_nMode!=4)
		pCmdUI->SetCheck(0);
	else
		pCmdUI->SetCheck(1);
}
