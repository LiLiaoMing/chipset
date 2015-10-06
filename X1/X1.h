
// X1.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CX1App:
// See X1.cpp for the implementation of this class
//

class CX1App : public CWinAppEx
{
public:
	CX1App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CX1App theApp;
extern char g_appPath[MAX_PATH];

void GetDlgItemRect(HWND hWnd, UINT id, CRect& rcItem);