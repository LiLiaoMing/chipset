#pragma once


#include "CardData.h"

// CInternalWnd dialog

class CInternalWnd : public CDialog
{
	DECLARE_DYNAMIC(CInternalWnd)

public:
	CInternalWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInternalWnd();

// Dialog Data
	enum { IDD = IDD_INTERNAL_DIALOG };

private:
	CxImage	m_img;
	int		m_nRBMode;		//RB mode
	BOOL	m_bTrack1Discr;

	int		m_nTrack2Length;	//28, 32 or 37
	CString	m_strOrTrack2Field1;
	CString	m_strOrTrack2Field2;

	CString m_strAID;
	CString m_strCountry;
	CString m_strCurrency;
	CString m_strPrefName;
	CString m_strEffectiveDate;
	CString m_strExpirationDate;
	CString m_strServiceCode;
	CArray<CCardData, CCardData&> m_ppCardData;
	int		m_nCardDataLen;
	CFile*	m_pFile; 

	CString GetTrack1Info();
	BOOL	GetTrack2Info();

	BOOL	CheckTrack2(CString& strTrack2);
	void	SplitTrack2(CString& strTrack2, CString& field1, CString& field2);

	void	ChangeStateItems();
	void	ChangeTrack1discr();
	void	ReadCardData();
	void	WriteCardData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void SerializeElements( CArchive& ar);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedRbMasterRadio();
	afx_msg void OnBnClickedRbMaster2Radio();
	afx_msg void OnBnClickedRbAmexRadio();
	afx_msg void OnBnClickedRbXRadio();
	afx_msg void OnBnClickedRbVisa2Radio();
	afx_msg void OnBnClickedRbDebit1Radio();
	afx_msg void OnBnClickedRbDebit2Radio();
	afx_msg void OnBnClickedRb6036Radio();
	afx_msg void OnBnClickedCreditButton();
	afx_msg void OnBnClickedDeditButton();
	afx_msg void OnBnClickedChnageTrack1discrCheck();
	afx_msg void OnBnClickedArqcThree();
	afx_msg void OnBnClickedArqcUnique();
	afx_msg void OnEnChangeArqc1Edit();
	afx_msg void OnBnClickedChangeCheck1();
	afx_msg void OnBnClickedChangeCheck2();
	afx_msg void OnBnClickedChangeCheck3();
	afx_msg void OnEnChangePrefNameEdit();
	afx_msg void OnEnChangeEffectiveDateEdit();
	afx_msg void OnEnChangeExpirationDateEdit();
	afx_msg void OnBnClickedIstGenButton();
	afx_msg void OnBnClickedSaveButton();
	afx_msg void OnEnChangeAidEdit();
	afx_msg void OnEnChangeCountryEdit2();
	afx_msg void OnEnChangeCurrencyEdit();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnCbnSelchangeCarddataCombo();
};
