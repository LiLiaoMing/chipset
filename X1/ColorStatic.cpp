// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "ColorStatic.h"


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic()
{
	m_TextColor = RGB(0, 0, 0);
	SetFontInfo(_T("Arial"),14,FALSE);
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CColorStatic message handlers




int CColorStatic::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void CColorStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.SetBkColor(m_BackColor);
	dc.SetBkMode(TRANSPARENT);
	CFont *pFont = &m_fntShow;//GetParent()->GetFont();
	CFont *pOldFont;
	pOldFont = dc.SelectObject(pFont);
	dc.SetTextColor(m_TextColor);
	dc.DrawText(m_strCaption, &rect, DT_CENTER);
	dc.SelectObject(pOldFont);
}

void CColorStatic::Create(CString strCaption, COLORREF BackColor)
{


}

void CColorStatic::SetCaption(CString strCaption)
{
	m_strCaption = strCaption;
}

void CColorStatic::SetBackColor(COLORREF BackColor)
{
	m_BackColor = BackColor;
}

void CColorStatic::SetTextColor(COLORREF TextColor)
{
	m_TextColor = TextColor;
}

void CColorStatic::SetFontInfo(char *pFontName, int nFntSize, BOOL bBold)
{
	m_fntShow.DeleteObject();

	m_fntShow.CreateFont(-nFntSize, 0, 0, 0, bBold ? FW_BOLD : FW_NORMAL, FALSE, FALSE,
						 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_STROKE_PRECIS,	//ANSI_CHARSET
						 ANTIALIASED_QUALITY, DEFAULT_PITCH, pFontName);	// DEFAULT_QUALITY
}