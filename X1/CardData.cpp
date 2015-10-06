// CardData.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "CardData.h"


// CCardData
IMPLEMENT_SERIAL(CCardData, CObject, 1);
CCardData::CCardData()
{
	m_strName = _T("");
	m_strAID = _T("");
	m_strCountry = _T("");
	m_strCurrency = _T("");
	m_strPrefName = _T("");
	m_strEffectiveDate = _T("");
	m_strExpirationDate = _T("");
}

CCardData::CCardData(CString strName, CString strAID, CString strCountry, CString strCurrency, CString strPrefName, CString strEffectiveDate, CString strExpirationDate)
{
	m_strName = strName;
	m_strAID = strAID;
	m_strCountry = strCountry;
	m_strCurrency = strCurrency;
	m_strPrefName = strPrefName;
	m_strEffectiveDate = strEffectiveDate;
	m_strExpirationDate = strExpirationDate;
}


CCardData::~CCardData()
{
}

void CCardData::Serialize(CArchive& ar)
{
	if(ar.IsLoading())
	{
		ar >> m_strName >> m_strAID >> m_strCountry >> m_strCurrency >> m_strPrefName >> m_strEffectiveDate >> m_strExpirationDate;
	}
	else
	{
		ar << m_strName << m_strAID << m_strCountry << m_strCurrency << m_strPrefName << m_strEffectiveDate << m_strExpirationDate;
	}
}


// CCardData member functions
