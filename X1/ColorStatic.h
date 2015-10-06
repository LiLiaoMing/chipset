#pragma once


// CColorStatic

class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();

	void SetTextColor(COLORREF TextColor);
	COLORREF m_TextColor;
	void SetBackColor(COLORREF BackColor);
	void SetCaption(CString strCaption);
	void Create(CString strCaption, COLORREF BackColor);
	void SetFontInfo(char *pFontName, int nFntSize, BOOL bBold=FALSE);

private:
	COLORREF m_BackColor;
	CString  m_strCaption;
	CFont	 m_fntShow;
	int		 m_nFntSize;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
};


