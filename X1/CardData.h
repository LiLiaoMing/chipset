#pragma once

// CCardData command target

class CCardData : public CObject
{
	DECLARE_SERIAL(CCardData);
public:
	CCardData();
	virtual ~CCardData();

	CCardData(CString strName, CString strAID, CString strCountry, CString strCurrency, CString strPrefName, CString strEffectiveDate, CString strExpirationDate);

	CCardData( const CCardData &s)
	{
		m_strName = s.m_strName;
		m_strAID = s.m_strAID;
		m_strCountry  = s.m_strCountry;
		m_strCurrency = s.m_strCurrency;
		m_strPrefName = s.m_strPrefName;
		m_strEffectiveDate = s.m_strEffectiveDate;
		m_strExpirationDate =s.m_strExpirationDate;
	}

	CCardData& operator=( const CCardData &s)
	{
		m_strName = s.m_strName;
		m_strAID = s.m_strAID;
		m_strCountry  = s.m_strCountry;
		m_strCurrency = s.m_strCurrency;
		m_strPrefName = s.m_strPrefName;
		m_strEffectiveDate = s.m_strEffectiveDate;
		m_strExpirationDate =s.m_strExpirationDate;
		return *this;
	}
	// Attributes
	
	virtual void Serialize(CArchive& ar);
	// Implementation
	// The user's name
	CString m_strName;
	CString m_strAID;
	CString m_strCountry;
	CString m_strCurrency;
	CString m_strPrefName;
	CString m_strEffectiveDate;
	CString m_strExpirationDate;
};


