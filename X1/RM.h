#pragma once

class CRM
{
public:
	CRM(void);
	~CRM(void);

	void SetCardInfo(CString& strTrack2, CString& strFolderName, CString& strAppLabel, BOOL bTrack1Discr, CString strTrack1, CString strAID, CString strCountry, CString strCurrency, CString strArqc1, CString strArqc2, CString strArqc3, CString strPrefName, CString strEffectiveDate, CString strExpirationDate, CString strServiceCode);

	CString GetTrack1Info(int nCategory);
	int		GetTrack2Info(int nCategory, CString& strFiled1, CString& strField2);

	void Burn(int nCategory){CStringArray ar; Burn(nCategory, ar, FALSE);};
	void Debit(int nCategory){CStringArray ar; Debit(nCategory, ar, FALSE);};
	void Burn(int nCategory, CStringArray& ar, BOOL isGen);
	void Debit(int nCategory, CStringArray& ar, BOOL isGen);
	void Format();

private:
	CString	m_strTrack2;
	CString	m_strCardFolderName;
	CString	m_strAppLabel;	//Application Label
	CString	m_strTrack1;
	BOOL	m_bTrack1Discr;
	CString	m_strAID;
	CString	m_strCountry;
	CString	m_strCurrency;
	CString	m_strArqc1;
	CString	m_strArqc2;
	CString	m_strArqc3;
	CString m_strPrefName;
	CString m_strEffectiveDate;
	CString m_strExpirationDate;
	CString m_strServiceCode;

	void MakeMasterScript01(CStringArray& sarray, BOOL isGen);
	void MakeMasterScript02(CStringArray& sarray, BOOL isGen);
	void MakeVisa2Script(CStringArray& sarray, BOOL isGen);
	void MakeXScript(CStringArray& sarray, BOOL isGen);
	void MakeAmexScript(CStringArray& sarray, BOOL isGen);

	void MakeDEBIT1Script(CStringArray& sarray, BOOL isGen);
	void MakeDEBIT2Script(CStringArray& sarray, BOOL isGen);

	CString GetTrack1_01();
	CString GetTrack1_02();
	CString GetTrack1_03();
	CString GetTrack1_04();
	CString GetTrack1_05();
	CString GetTrack1_06();
	CString GetTrack1_07();

};
