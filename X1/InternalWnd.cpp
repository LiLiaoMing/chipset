// InternalWnd.cpp : implementation file
//

#include "stdafx.h"
#include "X1.h"
#include "InternalWnd.h"
#include "RM.h"
#include "InputDlg.h"
#include "ConfirmDlg.h"

// CInternalWnd dialog

IMPLEMENT_DYNAMIC(CInternalWnd, CDialog)

CInternalWnd::CInternalWnd(CWnd* pParent /*=NULL*/)
	: CDialog(CInternalWnd::IDD, pParent)
{

}

CInternalWnd::~CInternalWnd()
{
}

void CInternalWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInternalWnd, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RB_MASTER_RADIO, &CInternalWnd::OnBnClickedRbMasterRadio)
	ON_BN_CLICKED(IDC_RB_MASTER2_RADIO, &CInternalWnd::OnBnClickedRbMaster2Radio)
	ON_BN_CLICKED(IDC_RB_AMEX_RADIO, &CInternalWnd::OnBnClickedRbAmexRadio)
	ON_BN_CLICKED(IDC_RB_X_RADIO, &CInternalWnd::OnBnClickedRbXRadio)
	ON_BN_CLICKED(IDC_RB_VISA2_RADIO, &CInternalWnd::OnBnClickedRbVisa2Radio)
	ON_BN_CLICKED(IDC_RB_DEBIT1_RADIO, &CInternalWnd::OnBnClickedRbDebit1Radio)
	ON_BN_CLICKED(IDC_RB_DEBIT2_RADIO, &CInternalWnd::OnBnClickedRbDebit2Radio)
	ON_BN_CLICKED(IDC_RB_6036_RADIO, &CInternalWnd::OnBnClickedRb6036Radio)
	ON_BN_CLICKED(IDC_CREDIT_BUTTON, &CInternalWnd::OnBnClickedCreditButton)
	ON_BN_CLICKED(IDC_DEDIT_BUTTON, &CInternalWnd::OnBnClickedDeditButton)
	ON_BN_CLICKED(IDC_CHNAGE_TRACK1DISCR_CHECK, &CInternalWnd::OnBnClickedChnageTrack1discrCheck)
	ON_BN_CLICKED(IDC_ARQC_THREE, &CInternalWnd::OnBnClickedArqcThree)
	ON_BN_CLICKED(IDC_ARQC_UNIQUE, &CInternalWnd::OnBnClickedArqcUnique)
	ON_EN_CHANGE(IDC_ARQC1_EDIT, &CInternalWnd::OnEnChangeArqc1Edit)
	ON_BN_CLICKED(IDC_CHANGE_CHECK1, &CInternalWnd::OnBnClickedChangeCheck1)
	ON_BN_CLICKED(IDC_CHANGE_CHECK2, &CInternalWnd::OnBnClickedChangeCheck2)
	ON_BN_CLICKED(IDC_CHANGE_CHECK3, &CInternalWnd::OnBnClickedChangeCheck3)
	ON_EN_CHANGE(IDC_PREF_NAME_EDIT, &CInternalWnd::OnEnChangePrefNameEdit)
	ON_EN_CHANGE(IDC_EFFECTIVE_DATE_EDIT, &CInternalWnd::OnEnChangeEffectiveDateEdit)
	ON_EN_CHANGE(IDC_EXPIRATION_DATE_EDIT, &CInternalWnd::OnEnChangeExpirationDateEdit)
	ON_BN_CLICKED(IDC_IST_GEN_BUTTON, &CInternalWnd::OnBnClickedIstGenButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CInternalWnd::OnBnClickedSaveButton)
	ON_EN_CHANGE(IDC_AID_EDIT, &CInternalWnd::OnEnChangeAidEdit)
	ON_EN_CHANGE(IDC_COUNTRY_EDIT2, &CInternalWnd::OnEnChangeCountryEdit2)
	ON_EN_CHANGE(IDC_CURRENCY_EDIT, &CInternalWnd::OnEnChangeCurrencyEdit)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CInternalWnd::OnBnClickedDeleteButton)
	ON_CBN_SELCHANGE(IDC_CARDDATA_COMBO, &CInternalWnd::OnCbnSelchangeCarddataCombo)
END_MESSAGE_MAP()


// CInternalWnd message handlers

BOOL CInternalWnd::OnInitDialog()
{
	CDialog::OnInitDialog();

	HRSRC handle = FindResource(NULL,MAKEINTRESOURCE(IDB_SMARTCARD_BITMAP),RT_BITMAP);
	BOOL bRet = m_img.LoadResource(handle,CXIMAGE_FORMAT_BMP);

	//Initialize variables
	m_nRBMode = 0;		//Master mode
	OnBnClickedChnageTrack1discrCheck();
	
	CButton* pBtn = (CButton*)GetDlgItem(IDC_RB_MASTER_RADIO);
	pBtn->SetCheck(1);

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_TRACK2_EDIT);
	pEdit->SetLimitText(37);

	pEdit = (CEdit*)GetDlgItem(IDC_AID_EDIT);
	pEdit->SetLimitText(5);
	pEdit = (CEdit*)GetDlgItem(IDC_COUNTRY_EDIT2);
	pEdit->SetLimitText(4);
	pEdit = (CEdit*)GetDlgItem(IDC_CURRENCY_EDIT);
	pEdit->SetLimitText(4);
	pEdit = (CEdit*)GetDlgItem(IDC_ARQC1_EDIT);
	pEdit->SetLimitText(16);
	pEdit = (CEdit*)GetDlgItem(IDC_ARQC2_EDIT);
	pEdit->SetLimitText(16);
	pEdit = (CEdit*)GetDlgItem(IDC_ARQC3_EDIT);
	pEdit->SetLimitText(16);
	pEdit = (CEdit*)GetDlgItem(IDC_PREF_NAME_EDIT);
	pEdit->EnableWindow(FALSE);
	pEdit->SetLimitText(14);
	pEdit = (CEdit*)GetDlgItem(IDC_EFFECTIVE_DATE_EDIT);
	pEdit->EnableWindow(FALSE);
	pEdit->SetLimitText(6);
	pEdit = (CEdit*)GetDlgItem(IDC_EXPIRATION_DATE_EDIT);
	pEdit->EnableWindow(FALSE);
	pEdit->SetLimitText(6);

	CButton* pButton = (CButton*)GetDlgItem(IDC_ARQC_THREE);
	pButton->SetCheck(true);

	m_strAID = _T("");
	m_strCountry = _T("");
	m_strCurrency = _T("");
	m_strPrefName = _T("4372656469746F");
	m_strEffectiveDate = _T("180118");
	m_strExpirationDate = _T("180131");
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
	pCombo->AddString(_T("Default"));
	pCombo->SetCurSel(0);
	ReadCardData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

	
}

void CInternalWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	CRect rcWork;

	GetDlgItemRect(this->m_hWnd, IDC_STATIC_PIC, rcWork);
	dc.SetBkMode(TRANSPARENT);
	m_img.Draw(dc.m_hDC, rcWork);
}

CString CInternalWnd::GetTrack1Info()
{
	CString strTrack1;
	CRM card;

	strTrack1 = card.GetTrack1Info(m_nRBMode);

	return strTrack1;
}

BOOL CInternalWnd::GetTrack2Info()
{
	m_strOrTrack2Field1 = _T("");
	m_strOrTrack2Field2 = _T("");
	m_nTrack2Length = -1;

	CRM card;

	m_nTrack2Length = card.GetTrack2Info(m_nRBMode, m_strOrTrack2Field1, m_strOrTrack2Field2);

	return (m_nTrack2Length>0)? TRUE:FALSE;
}

void CInternalWnd::SplitTrack2(CString& strTrack2, CString& field1, CString& field2)
{
	int		pos;
	CString	strTrack22 = strTrack2;

	strTrack22.MakeUpper();

	pos = strTrack22.Find(_T("="));
	if(pos>0){
		field1 = strTrack22.Left(pos);
		field2 = strTrack22.Mid(pos+1);
	}
}

BOOL CInternalWnd::CheckTrack2(CString& strTrack2)
{
	CString strField1;
	CString strField2;
	CString strMsg;

	int ndifLen;

	if( !strTrack2.IsEmpty()) {

		if( !GetTrack2Info()) {
			MessageBox(_T("The ist file is incorrect!"), _T("Error"), MB_ICONERROR|MB_OK);
			return FALSE;
		}

		SplitTrack2(strTrack2, strField1, strField2);

		// Check out the first field
		ndifLen = strField1.GetLength() - m_strOrTrack2Field1.GetLength();
		if(ndifLen>0) {

			strMsg.Format(_T("The feild(1) is long as [%d]"), ndifLen);
			MessageBox(strMsg, _T("Error"), MB_ICONERROR|MB_OK);
			return FALSE;
		}
		else if(ndifLen<0) {
			strMsg.Format(_T("The feild(1) is short as [%d]"), -ndifLen);
			MessageBox(strMsg, _T("Error"), MB_ICONERROR|MB_OK);
			return FALSE;
		}

		// Check out the second field
		ndifLen = strField2.GetLength() - m_strOrTrack2Field2.GetLength();
		if(ndifLen>0) {

			strMsg.Format(_T("The feild(2) is long as [%d]"), ndifLen);
			MessageBox(strMsg, _T("Error"), MB_ICONERROR|MB_OK);
			return FALSE;
		}
		else if(ndifLen<0) {
			if(ndifLen == -1 && strField2.GetLength()%2==0) {
				if(m_nTrack2Length*2 == 38)
					strField2.Append(_T("F"));
				else {
					if( MessageBox(_T(" Input information omit one! \n You want to add [F] at last?(Yes/No)"), _T("Question"), MB_ICONQUESTION|MB_YESNO) == IDYES)
						strField2.Append(_T("F"));
					else {
						strMsg.Format(_T("The feild(2) is short as [%d]"), -ndifLen);
						MessageBox(strMsg, _T("Error"), MB_ICONERROR|MB_OK);
						return FALSE;
					}
				}
			}
			else {

				if(m_nTrack2Length*2>32)
					ndifLen += 1;
				strMsg.Format(_T("The feild(2) is short as [%d]"), -ndifLen);
				MessageBox(strMsg, _T("Error"), MB_ICONERROR|MB_OK);
				return FALSE;
			}
		}

		strTrack2.Format(_T("%s=%s"),strField1, strField2);
	}
	return TRUE;
}

void CInternalWnd::ChangeStateItems()
{
	CWnd* pWnd;
	BOOL  bState = FALSE;

	if(m_nRBMode>=0 && m_nRBMode<=4)
		bState = TRUE;
	else
		bState = FALSE;

	pWnd = GetDlgItem(IDC_CREDIT_BUTTON);
	pWnd->EnableWindow(bState);

	pWnd = GetDlgItem(IDC_DEDIT_BUTTON);
	pWnd->EnableWindow(!bState);

	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbMasterRadio()
{
	m_nRBMode = 0;	//MASTERCARD
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbMaster2Radio()
{
	m_nRBMode = 1;	//MASTERCARD 2
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbAmexRadio()
{
	m_nRBMode = 2;	//AMEX
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbXRadio()
{
	m_nRBMode = 3;	//X
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbVisa2Radio()
{
	m_nRBMode = 4;	//VISA2
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbDebit1Radio()
{
	m_nRBMode = 5;	//DEBIT 1
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRbDebit2Radio()
{
	m_nRBMode = 6;	//DEBIT 2
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::OnBnClickedRb6036Radio()
{
	m_nRBMode = 7;	//6036
	OnBnClickedChnageTrack1discrCheck();
}

void CInternalWnd::ChangeTrack1discr()
{
	CString strTrack1 = GetTrack1Info();
	CWnd* pWnd = GetDlgItem(IDC_TRACK1_EDIT);

	if(strTrack1.IsEmpty())
		pWnd->EnableWindow(FALSE);
	else
		pWnd->EnableWindow(TRUE);

	SetDlgItemText(IDC_TRACK1_EDIT, strTrack1);
}

void CInternalWnd::OnBnClickedChnageTrack1discrCheck()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHNAGE_TRACK1DISCR_CHECK);

	m_bTrack1Discr = pBtn->GetCheck();

	if(m_bTrack1Discr) {
		GetDlgItem(IDC_TRACK1_EDIT)->EnableWindow();
		ChangeTrack1discr();
	}
	else {
		SetDlgItemText(IDC_TRACK1_EDIT, _T(""));	
		GetDlgItem(IDC_TRACK1_EDIT)->EnableWindow(FALSE);
	}

	if (m_nRBMode == 0)
		m_strEffectiveDate = _T("180118");
	else
		m_strEffectiveDate = _T("140310");
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
	//if (pCombo->GetCurSel() != 0 )
	//	SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, m_strEffectiveDate);

}

void CInternalWnd::OnBnClickedCreditButton()
{
	CString strTrack2Info = _T("");
	CString strCardFolderName = _T("");
	CString strAppLabel = _T("");
	CString strTrack1 = _T("");
	CString strAID = _T("");
	CString strCountry = _T("");
	CString strCurrency = _T("");
	CString strArqc1 = _T("");
	CString strArqc2 = _T("");
	CString strArqc3 = _T("");

	GetDlgItemText(IDC_TRACK2_EDIT, strTrack2Info);
	GetDlgItemText(IDC_HOLDERNAME_EDIT, strCardFolderName);
	GetDlgItemText(IDC_APP_LABEL_EDIT, strAppLabel);
	GetDlgItemText(IDC_TRACK1_EDIT, strTrack1);
	GetDlgItemText(IDC_AID_EDIT, strAID);
	GetDlgItemText(IDC_COUNTRY_EDIT2, strCountry);
	GetDlgItemText(IDC_CURRENCY_EDIT, strCurrency);
	GetDlgItemText(IDC_ARQC1_EDIT, strArqc1);
	GetDlgItemText(IDC_ARQC2_EDIT, strArqc2);
	GetDlgItemText(IDC_ARQC3_EDIT, strArqc3);

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHNAGE_TRACK1DISCR_CHECK);
	BOOL bTrack1Discr = pBtn->GetCheck();

	if( !CheckTrack2(strTrack2Info))
		return;
	m_strServiceCode = strTrack2Info.Mid(21, 3);
	m_strServiceCode = _T("0") + m_strServiceCode;

	CheckLabelValidate(strAppLabel);

	pBtn = (CButton*)GetDlgItem(IDC_EMPTY_HOLDERNAME_CHECK);
	int bEraseHolderName = pBtn->GetCheck();

	if(bEraseHolderName) {
		for( int i=0;i<30;i++){
			strCardFolderName.AppendChar(0x20);
		}
	}

	if (strAID.GetLength() == 0)
		strAID = _T("41010");
	
	if (strCountry.GetLength() == 0)
		strCountry = _T("0076");
	
	if (strCurrency.GetLength() == 0)
		strCurrency = _T("0986");
	
	if (strArqc1.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc1 = _T("8A3470B2406A1148");
		else
			strArqc1 = _T("4BDCB1DA04AFF5CC");
	}
	if (strArqc2.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc2 = _T("BA9A9D755485B3EC");
		else
			strArqc2 = _T("65DADA0970FE6E64");
	}
	if (strArqc3.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc3 = _T("8A3470B2406A1148");
		else
			strArqc3 = _T("4BDCB1DA04AFF5CC");
	}

	if(m_nRBMode >= 0) {
		CRM card;
		card.SetCardInfo(strTrack2Info, strCardFolderName, strAppLabel, bTrack1Discr, strTrack1, strAID, strCountry, strCurrency, strArqc1, strArqc2, strArqc3, m_strPrefName, m_strEffectiveDate, m_strExpirationDate, m_strServiceCode);
		card.Format();
		card.Burn(m_nRBMode);
		//m_nRBMode = -1;
	}

	MessageBox(_T("Completed job!"), _T("Result"), MB_ICONINFORMATION|MB_OK);
}

void CInternalWnd::OnBnClickedDeditButton()
{
	CString strTrack2Info;
	CString strCardFolderName;
	CString strAppLabel;
	CString strTrack1 = _T("");
	CString strAID = _T("");
	CString strCountry = _T("");
	CString strCurrency = _T("");
	CString strArqc1 = _T("");
	CString strArqc2 = _T("");
	CString strArqc3 = _T("");

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHNAGE_TRACK1DISCR_CHECK);
	BOOL bTrack1Discr = pBtn->GetCheck();

	GetDlgItemText(IDC_TRACK2_EDIT, strTrack2Info);
	GetDlgItemText(IDC_HOLDERNAME_EDIT, strCardFolderName);
	GetDlgItemText(IDC_APP_LABEL_EDIT, strAppLabel);
	GetDlgItemText(IDC_TRACK1_EDIT, strTrack1);
	GetDlgItemText(IDC_AID_EDIT, strAID);
	GetDlgItemText(IDC_COUNTRY_EDIT2, strCountry);
	GetDlgItemText(IDC_CURRENCY_EDIT, strCurrency);
	GetDlgItemText(IDC_ARQC1_EDIT, strArqc1);
	GetDlgItemText(IDC_ARQC2_EDIT, strArqc2);
	GetDlgItemText(IDC_ARQC3_EDIT, strArqc3);

	if( !CheckTrack2(strTrack2Info))
		return;
	m_strServiceCode = strTrack2Info.Mid(21, 3);
	m_strServiceCode = _T("0") + m_strServiceCode;

	CheckLabelValidate(strAppLabel);

	pBtn = (CButton*)GetDlgItem(IDC_EMPTY_HOLDERNAME_CHECK);
	int bEraseHolderName = pBtn->GetCheck();

	if(bEraseHolderName) {
		for( int i=0;i<30;i++){
			strCardFolderName.AppendChar(0x20);
		}
	}

	if (strAID.GetLength() == 0)
		strAID = _T("41010");

	if (strCountry.GetLength() == 0)
		strCountry = _T("0076");

	if (strCurrency.GetLength() == 0)
		strCurrency = _T("0986");

	if (strArqc1.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc1 = _T("8A3470B2406A1148");
		else
			strArqc1 = _T("4BDCB1DA04AFF5CC");
	}
	if (strArqc2.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc2 = _T("BA9A9D755485B3EC");
		else
			strArqc2 = _T("65DADA0970FE6E64");
	}
	if (strArqc3.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc3 = _T("8A3470B2406A1148");
		else
			strArqc3 = _T("4BDCB1DA04AFF5CC");
	}

	if(m_nRBMode >= 0) {
		if(strTrack2Info.IsEmpty()) {
			MessageBox(_T("Please input Track2Info field!"), _T("Error"), MB_ICONERROR|MB_OK);
			return;
		}

		CRM card;
		card.SetCardInfo(strTrack2Info, strCardFolderName, strAppLabel, bTrack1Discr, strTrack1, strAID, strCountry, strCurrency, strArqc1, strArqc2, strArqc3, m_strPrefName, m_strEffectiveDate, m_strExpirationDate, m_strServiceCode);
		card.Format();
		card.Debit(m_nRBMode-5);
		//m_nRBMode = -1;
	}

	MessageBox(_T("Completed job!"), _T("Result"), MB_ICONINFORMATION|MB_OK);
}



void CInternalWnd::OnBnClickedArqcThree()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ARQC2_EDIT);
	pEdit->EnableWindow(TRUE);
	pEdit = (CEdit*)GetDlgItem(IDC_ARQC3_EDIT);
	pEdit->EnableWindow(TRUE);
}

void CInternalWnd::OnBnClickedArqcUnique()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_ARQC2_EDIT);
	pEdit->EnableWindow(FALSE);
	pEdit = (CEdit*)GetDlgItem(IDC_ARQC3_EDIT);
	pEdit->EnableWindow(FALSE);
}

void CInternalWnd::OnEnChangeArqc1Edit()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_ARQC_UNIQUE);
	if (!pButton->GetCheck())
		return;
	
	CString str;
	GetDlgItemText(IDC_ARQC1_EDIT, str);
	SetDlgItemText(IDC_ARQC2_EDIT, str);
	SetDlgItemText(IDC_ARQC3_EDIT, str);
}

void CInternalWnd::OnBnClickedChangeCheck1()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHANGE_CHECK1);

	if( pBtn->GetCheck() ) {
		GetDlgItem(IDC_PREF_NAME_EDIT)->EnableWindow();
		SetDlgItemText(IDC_PREF_NAME_EDIT, _T("4372656469746F"));
	}
	else {
		SetDlgItemText(IDC_PREF_NAME_EDIT, _T(""));	
		GetDlgItem(IDC_PREF_NAME_EDIT)->EnableWindow(FALSE);
	}
}

void CInternalWnd::OnBnClickedChangeCheck2()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHANGE_CHECK2);

	if( pBtn->GetCheck() ) {
		GetDlgItem(IDC_EFFECTIVE_DATE_EDIT)->EnableWindow();
		if (m_nRBMode == 0)
			SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, _T("180118"));
		else
			SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, _T("140310"));
	}
	else {
		SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, _T(""));	
		GetDlgItem(IDC_EFFECTIVE_DATE_EDIT)->EnableWindow(FALSE);
	}
}

void CInternalWnd::OnBnClickedChangeCheck3()
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHANGE_CHECK3);

	if( pBtn->GetCheck() ) {
		GetDlgItem(IDC_EXPIRATION_DATE_EDIT)->EnableWindow();
		SetDlgItemText(IDC_EXPIRATION_DATE_EDIT, _T("180131"));
	}
	else {
		SetDlgItemText(IDC_EXPIRATION_DATE_EDIT, _T(""));	
		GetDlgItem(IDC_EXPIRATION_DATE_EDIT)->EnableWindow(FALSE);
	}
}

void CInternalWnd::OnEnChangePrefNameEdit()
{
	CString str;
	GetDlgItemText(IDC_PREF_NAME_EDIT, str);
	if(str.GetLength() == 0)
		return;
	m_strPrefName = str;
}

void CInternalWnd::OnEnChangeEffectiveDateEdit()
{
	CString str;
	GetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, str);
	if(str.GetLength() == 0)
		return;
	m_strEffectiveDate = str;
}

void CInternalWnd::OnEnChangeExpirationDateEdit()
{
	CString str;
	GetDlgItemText(IDC_EXPIRATION_DATE_EDIT, str);
	if(str.GetLength() == 0)
		return;
	m_strExpirationDate = str;
}

void CInternalWnd::OnBnClickedIstGenButton()
{
	CStringArray arr;

	CString strTrack2Info;
	CString strCardFolderName;
	CString strAppLabel;
	CString strTrack1 = _T("");
	CString strAID = _T("");
	CString strCountry = _T("");
	CString strCurrency = _T("");
	CString strArqc1 = _T("");
	CString strArqc2 = _T("");
	CString strArqc3 = _T("");

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHNAGE_TRACK1DISCR_CHECK);
	BOOL bTrack1Discr = pBtn->GetCheck();

	GetDlgItemText(IDC_TRACK2_EDIT, strTrack2Info);
	GetDlgItemText(IDC_HOLDERNAME_EDIT, strCardFolderName);
	GetDlgItemText(IDC_APP_LABEL_EDIT, strAppLabel);
	GetDlgItemText(IDC_TRACK1_EDIT, strTrack1);
	GetDlgItemText(IDC_AID_EDIT, strAID);
	GetDlgItemText(IDC_COUNTRY_EDIT2, strCountry);
	GetDlgItemText(IDC_CURRENCY_EDIT, strCurrency);
	GetDlgItemText(IDC_ARQC1_EDIT, strArqc1);
	GetDlgItemText(IDC_ARQC2_EDIT, strArqc2);
	GetDlgItemText(IDC_ARQC3_EDIT, strArqc3);

	if( !CheckTrack2(strTrack2Info))
		return;
	
	m_strServiceCode = strTrack2Info.Mid(21, 3);
	m_strServiceCode = _T("0") + m_strServiceCode;

	CheckLabelValidate(strAppLabel);

	pBtn = (CButton*)GetDlgItem(IDC_EMPTY_HOLDERNAME_CHECK);
	int bEraseHolderName = pBtn->GetCheck();

	if(bEraseHolderName) {
		for( int i=0;i<30;i++){
			strCardFolderName.AppendChar(0x20);
		}
	}

	if (strAID.GetLength() == 0)
		strAID = _T("41010");

	if (strCountry.GetLength() == 0)
		strCountry = _T("0076");

	if (strCurrency.GetLength() == 0)
		strCurrency = _T("0986");

	if (strArqc1.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc1 = _T("8A3470B2406A1148");
		else
			strArqc1 = _T("4BDCB1DA04AFF5CC");
	}
	if (strArqc2.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc2 = _T("BA9A9D755485B3EC");
		else
			strArqc2 = _T("65DADA0970FE6E64");
	}
	if (strArqc3.GetLength() == 0)
	{
		if (m_nRBMode == 0)
			strArqc3 = _T("8A3470B2406A1148");
		else
			strArqc3 = _T("4BDCB1DA04AFF5CC");
	}

	if(m_nRBMode >= 0 ) {
		if(strTrack2Info.IsEmpty()) {
			MessageBox(_T("Please input Track2Info field!"), _T("Error"), MB_ICONERROR|MB_OK);
			return;
		}

		CRM card;
		card.SetCardInfo(strTrack2Info, strCardFolderName, strAppLabel, bTrack1Discr, strTrack1, strAID, strCountry, strCurrency, strArqc1, strArqc2, strArqc3, m_strPrefName, m_strEffectiveDate, m_strExpirationDate, m_strServiceCode);
		card.Format();
		
		if(m_nRBMode > 4)
			card.Debit(m_nRBMode-5, arr, TRUE);
		else
			card.Burn(m_nRBMode, arr, TRUE);
		
	}

	/* =================================================== */
	CString strFileName = _T("");

	CFileDialog dlg(FALSE, _T("IST"));
	if(dlg.DoModal() == IDOK) {
		strFileName = dlg.GetPathName();
	}

	if(strFileName.IsEmpty()) {
		MessageBox(_T("Please select file name!"), _T("Error"), MB_ICONERROR|MB_OK);
		return;
	}

	CStdioFile file;
	if( !file.Open(strFileName, CStdioFile::modeCreate|CStdioFile::modeReadWrite)) {
		MessageBox(_T("Can't create ist file!"), _T("Error"), MB_ICONERROR|MB_OK);
		return;
	}
	
	CString strAPDU;
	char szLRLF[2] = {0x0a,0};

	for( int i=0;i<arr.GetSize();i++) {
		strAPDU = arr.GetAt(i);
		file.WriteString(strAPDU);
		file.WriteString(szLRLF);
	}

	file.Close();

	MessageBox(_T("The job completed!"), _T("Success"), MB_ICONINFORMATION|MB_OK);
}

void CInternalWnd::OnBnClickedSaveButton()
{
	CString strName;

	CInputDlg dialog;
	if (dialog.DoModal() == IDOK) {
		strName = dialog.m_strName;
		if(strName.GetLength() == 0)
			return;

		CCardData* pData = new CCardData(strName, m_strAID, m_strCountry, m_strCurrency, m_strPrefName, m_strEffectiveDate, m_strExpirationDate);
		m_ppCardData.Add(*pData);
		//m_ppCardData[m_nCardDataLen] = pData;
		
		//m_nCardDataLen++;
		WriteCardData();
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
		pCombo->InsertString(pCombo->GetCount(), strName);
	}
	
	MessageBox(_T("Saved successfully!"), _T("Result"), MB_ICONINFORMATION|MB_OK);
}

void CInternalWnd::SerializeElements( CArchive& ar)
{
	int nCount = m_ppCardData.GetSize();
	
	if( ar.IsStoring() )
	{
		ar << nCount;
	}
	else
	{
		ar >> nCount;
		m_ppCardData.RemoveAll();
	}
	
	for( int i = 0; i < nCount; i++)
	{

		if( ar.IsStoring() )
		{
			m_ppCardData[i].Serialize(ar);
		}
		else
		{
			CCardData* pCardData = new CCardData;
			pCardData->Serialize(ar);
			m_ppCardData.Add(*pCardData);
			
		}
	}
	
	if (ar.IsStoring())
		ar.Flush();
	if(m_pFile)
	{
		m_pFile->Close();
		m_pFile = NULL;
	}
}

void CInternalWnd::ReadCardData()
{
	CString fileName = _T("cardtemp.dat");
	
	CFile* pFile = new CFile();
	ASSERT (pFile != NULL);
	if (!pFile->Open (fileName, CFile::modeReadWrite | CFile::shareExclusive)) {
		m_nCardDataLen = 0;
		return;
	}
	m_pFile = pFile;

	CArchive* pArchive = NULL;
	try
	{
		pFile->SeekToBegin();
		UINT uMode = CArchive::load;
		pArchive = new CArchive (pFile, uMode);
		ASSERT (pArchive != NULL);
	}
	catch (CException* pException)
	{
		m_nCardDataLen = 0;
		return;
	}
	
	SerializeElements(*pArchive);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
	
	for(int i=0; i<m_ppCardData.GetSize(); i++) {
		pCombo->InsertString(i+1, m_ppCardData[i].m_strName);
	}

}

void CInternalWnd::WriteCardData()
{
	CString fileName = _T("cardtemp.dat");
	try
	{
		CFile::Remove(fileName);
	}
	catch (CFileException* pEx)
	{
		pEx->Delete();
	}

	CFile* pFile = new CFile();
	ASSERT (pFile != NULL);
	pFile->Open(fileName, CFile::modeCreate|CFile:: modeReadWrite);
	m_pFile = pFile;

	CArchive* pArchive = NULL;
	try
	{
		pFile->SeekToBegin();
		UINT uMode = CArchive::store;
		pArchive = new CArchive (pFile, uMode);
		ASSERT (pArchive != NULL);
	}
	catch (CException* pException)
	{
		return;
	}
	
	SerializeElements(*pArchive);

}

void CInternalWnd::OnEnChangeAidEdit()
{
	GetDlgItemText(IDC_AID_EDIT, m_strAID);
}

void CInternalWnd::OnEnChangeCountryEdit2()
{
	GetDlgItemText(IDC_COUNTRY_EDIT2, m_strCountry);
}

void CInternalWnd::OnEnChangeCurrencyEdit()
{
	GetDlgItemText(IDC_CURRENCY_EDIT, m_strCurrency);
}

void CInternalWnd::OnBnClickedDeleteButton()
{
	CConfirmDlg dialog;
	if (dialog.DoModal() == IDOK) {

		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
		int ix = pCombo->GetCurSel();
		if (ix == 0)
			return;
		m_ppCardData.RemoveAt(ix-1);
		WriteCardData();
		
		pCombo->DeleteString(ix);
		pCombo->SetCurSel(0);
	}
}

void CInternalWnd::OnCbnSelchangeCarddataCombo()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_CARDDATA_COMBO);
	int idx = pCombo->GetCurSel();
	if (idx == 0)
	{
		SetDlgItemText(IDC_AID_EDIT, _T(""));
		SetDlgItemText(IDC_COUNTRY_EDIT2, _T(""));
		SetDlgItemText(IDC_CURRENCY_EDIT, _T(""));
		SetDlgItemText(IDC_PREF_NAME_EDIT, _T(""));
		SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, _T(""));
		SetDlgItemText(IDC_EXPIRATION_DATE_EDIT, _T(""));
	}
	else 
	{
		idx--;
		SetDlgItemText(IDC_AID_EDIT, m_ppCardData[idx].m_strAID);
		SetDlgItemText(IDC_COUNTRY_EDIT2, m_ppCardData[idx].m_strCountry);
		SetDlgItemText(IDC_CURRENCY_EDIT, m_ppCardData[idx].m_strCurrency);
		SetDlgItemText(IDC_PREF_NAME_EDIT, m_ppCardData[idx].m_strPrefName);
		SetDlgItemText(IDC_EFFECTIVE_DATE_EDIT, m_ppCardData[idx].m_strEffectiveDate);
		SetDlgItemText(IDC_EXPIRATION_DATE_EDIT, m_ppCardData[idx].m_strExpirationDate);
	}
}
