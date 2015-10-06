#include "stdafx.h"
#include "Common.h"

//usage: 
//  BYTE g_recvAPDU[256];
//	int  g_recvAPDULen;
//	ConvertBytesToHexString( g_recvAPDULen, g_recvAPDU, true )
CString ConvertBytesToHexStringEx(int iLength, BYTE * pbInput, BOOL bDivider)
{
	if (iLength > 0)
	{
		if (pbInput != NULL)
		{
			CString szOutput;
			if (bDivider)
			{
				for (int iCount = 0; iCount < iLength; iCount++)
					szOutput.AppendFormat("%02X ", *(BYTE *)(pbInput+iCount));
			}
			else
			{
				for (int iCount = 0; iCount < iLength; iCount++)
					szOutput.AppendFormat("%02X", *(BYTE *)(pbInput+iCount));
			}
			return szOutput;
		}
		else
			return "";
	}
	else
		return "";

}

void ConvertHexCharToByte(char * pbInput, BYTE *pOut)
{
	char* pEnd;

	int val = strtol(pbInput, &pEnd, 16);

	*pOut = (BYTE)val;
}

void ConvertHexCharToInt(char * pbInput, int *pOut)
{
	char* pEnd;

	int val = strtol(pbInput, &pEnd, 16);

	*pOut = val;
}

void ConvertHexStringToBytes(char * pbInput, BYTE *pOut)
{
	int k, len = strlen(pbInput);
	char ch[3] = {0,};
	LPBYTE lpDst = pOut;

	for( k=0;k<len;k+=2) {
		ch[0] = pbInput[k];	
		ch[1] = pbInput[k+1];	
		ch[2] = 0;

		ConvertHexCharToByte(ch, lpDst);
		lpDst ++;
	}
}

char* FindCountryInCountryList(char* pCountryCode)
{
	int idx = 0;
	int ccode, ccode2;
	char* p = NULL;
	BOOL bFound = FALSE;

	ConvertHexCharToInt(pCountryCode, &ccode2);

	while(1) {
		p = (char*)countryList[idx][0];
		if(p==NULL) break;

		ConvertHexCharToInt(p, &ccode);
		if( ccode == ccode2) {
			bFound = TRUE;
			break;
		}
		idx ++;
	}

	if(bFound)
		return (char*)countryList[idx][1];
	return NULL;
}

char* FindCountryCodeInCountryList(char* pCountry)
{
	int idx = 0;
	char* p = NULL;
	BOOL bFound = FALSE;

	while(1) {
		p = (char*)countryList[idx][1];
		if(p==NULL) break;

		if( !strcmp(p, pCountry)) {
			bFound = TRUE;
			break;
		}
		idx ++;
	}

	if(bFound)
		return (char*)countryList[idx][0];
	return NULL;
}

BOOL CheckLabelValidate(CString& strVal)
{


	int valLen = strVal.GetLength();
	if(valLen == 0)
		return FALSE;

	if( valLen > 10) {
		strVal.Delete(10, valLen-10);
	}
	else if(valLen<10) {
		for( int n=0;n<10-valLen;n++)
			strVal += _T(" ");
	}

	return TRUE;
}

int FindOneByteTagPos(CString& strAPDU, CString strBaseTag, CString strFindTag, int LenLimit)
{
	int pos;
	BYTE lenTag;
	CString strTemp;
	BOOL bFound = FALSE;

	pos = strAPDU.Find(strBaseTag);
	if(pos>0) {

		//may be, there are multime tags in APDU command
		while (1)
		{
			pos = strAPDU.Find(strFindTag, pos+2);
			if(pos>0) {
				strTemp = strAPDU.Mid(pos+2, 2);	//Get length;
				ConvertHexCharToByte((char*)(LPCTSTR)strTemp, &lenTag);
				if(lenTag>LenLimit/2 && lenTag<=LenLimit){
					bFound = TRUE;
					break;
				}
				continue;
			}
			break;
		}
	}
	if(bFound)
		return pos;
	return -1;
}

int GetTagFieldByteLength(CString strAPDU, CString strTag)
{
	CString strTmp;
	int len=0, pos = strAPDU.Find(strTag);

	if(pos>0) {
		strTmp = strAPDU.Mid(pos+strTag.GetLength(), 2);	// 16 radix

		ConvertHexCharToInt((char*)(LPCTSTR)strTmp, &len);
	}
	return len;
}

void AdjustInfo(CString& strInfo, int nTagLen)
{
	int valLen = strInfo.GetLength();

	if( valLen > nTagLen) {
		strInfo.Delete(nTagLen, valLen-nTagLen);
	}
	else if(valLen<nTagLen) {
		for( int n=0;n<nTagLen-valLen;n++)
			strInfo += _T(" ");
	}
}

void ReplaceSubStrings(CString strAPDU, CString strSubString, int tagPos, int offset)
{
	int nn, n, len = strSubString.GetLength();
	char buf[2];
	char buf2[2];
	char *p = (char*)(LPCTSTR)strSubString;
	char *pp = (char*)(LPCTSTR)strAPDU;

	tagPos += offset;//A526500A
	for( nn=tagPos,n=0;n<len;n++) {

		buf[0] = p[n]>>4;
		buf[1] = p[n]&0xf;

		_itoa(buf[0], buf2, 16);
		pp[nn++] = buf2[0];
		_itoa(buf[1], buf2, 16);
		pp[nn++] = buf2[0];
	}
}

void SeparateAID(CString strAID, CString& strField1, CString& strField2, CString& strFiled22)
{
	int pos = strAID.Find(_T("="));

	if(pos>0) {
		strField1 = strAID.Left( pos);
		strField2 = strAID.Mid(pos+1);
		strFiled22 = strField2.Mid(7);
	}
}

BOOL ReplaceInfoEx(CString& strAPDU, CString strTag, CString& strRelplace)
{
	int lenField;
	int pos, pos1;
	CString strTemp;
	CString strField1;
	CString strField2;
	CString strField22;
	CString strAPDU2 = strAPDU;

	strAPDU2.MakeUpper();

	if( !strTag.Compare(_T("5F20"))){		//CardHolderName
		pos = strAPDU2.Find(strTag);

		if(pos>0) {

			lenField = GetTagFieldByteLength(strAPDU2, strTag);
			AdjustInfo(strRelplace, lenField);
			ReplaceSubStrings(strAPDU, strRelplace, pos, strTag.GetLength()+2);	//2 is length of HolderName

			return TRUE;
		}
	}
	else if( !strTag.Compare(_T("50"))) {	//Application Label
		pos = strAPDU2.Find(_T("500A"));	//0A : length

		if(pos>0) {
			AdjustInfo(strRelplace, 10);
			ReplaceSubStrings(strAPDU, strRelplace, pos, strTag.GetLength()+2);	//2 is length of HolderName
			return TRUE;
		}
	}
	else if( !strTag.Compare(_T("57"))) {	//AID

		BYTE len;

		SeparateAID(strRelplace, strField1, strField2, strField22);
		if(strField1.IsEmpty() || strField2.IsEmpty() || strField22.IsEmpty()) 
			return FALSE;

		pos = strAPDU2.Find(_T("00B20"));
		if(pos>0) {

			pos1 = FindOneByteTagPos(strAPDU2, _T("70"), strTag, 20);	//AAADBBB - length : 13
			if(pos1>0) {

				strTemp = strAPDU2.Mid(pos1+2,2);	//First field of AID length
				ConvertHexCharToByte((char*)(LPCTSTR)strTemp, &len);

				if(len == (strField1.GetLength()/2+strField2.GetLength()/2+1)) // AAAAADBBBBB
				{
					strAPDU.Delete(pos1+2, len*2+2);	//19(0x13)*2
					strTemp.Format(_T("%02X%sD%s"), strRelplace.GetLength()/2, strField1, strField2);
					strAPDU.Insert(pos1+2, strTemp);
					return TRUE;
				}
			}
			//Step 2 (Replace AAAAA) - if have not '70' tag
			else 
			{
				len = 8;
				pos1 = strAPDU2.Find(_T("5A08"), pos+1);		//MASTER or VISA
				if(pos1<0) {
					pos1 = strAPDU2.Find(_T("5A07"), pos+1);	//AMEX
					len = 7;
				}

				if(pos1>0) {
					//strTemp = strAPDU2.Mid(pos1+4,2);	//First field of AID length
					//ConvertHexCharToByte((char*)(LPCTSTR)strTemp, &len);

					if(len == (strField1.GetLength()/2)) // AAAAA
					{
						strAPDU.Delete(pos1+4, len*2);	//19(0x13)*2
						strAPDU.Insert(pos1+4, strField1);
						return TRUE;
					}
				}
			}
		}
	}


	else if( !strTag.Compare(_T("9F1F"))) {	//Track 1 Discretionary

		//SeparateAID(strRelplace, strField1, strField2, strField22);
		//if(strField1.IsEmpty() || strField2.IsEmpty() || strField22.IsEmpty()) 
		//	return FALSE;

		if(strRelplace.IsEmpty()) return FALSE;

		strField22 = strRelplace;	//manual track1 information

		pos = strAPDU2.Find(strTag);
		if(pos>0) {
			//TO DO
			lenField = GetTagFieldByteLength(strAPDU2, strTag);
			AdjustInfo(strField22, lenField);
			ReplaceSubStrings(strAPDU, strField22, pos, strTag.GetLength()+2);	//2 is length

			return TRUE;
		}
	}
	else if( !strTag.Compare(_T("9F20"))) {	//Track 2 Discretionary

		SeparateAID(strRelplace, strField1, strField2, strField22);
		if(strField1.IsEmpty() || strField2.IsEmpty() || strField22.IsEmpty()) 
			return FALSE;

		pos = strAPDU2.Find(strTag);
		if(pos>0) {

			lenField = GetTagFieldByteLength(strAPDU2, strTag);

			int len = strField22.GetLength();
			pos += strTag.GetLength()+2;//2 is field length

			strAPDU.Delete(pos, len);
			strAPDU.Insert(pos, strField22);

			return TRUE;
		}
	}
	else if( !strTag.Compare(_T("5F28")) || !strTag.Compare(_T("9F42"))) {

		if(strRelplace.IsEmpty()) return FALSE;

		strField22 = strRelplace;

		pos = strAPDU2.Find(strTag);
		if(pos>0) {
			//TO DO
			lenField = GetTagFieldByteLength(strAPDU2, strTag);
			AdjustInfo(strField22, lenField);
			ReplaceSubStrings(strAPDU, strField22, pos, strTag.GetLength()+2);	//2 is length

			return TRUE;
		}
	}
	else if( !strTag.Compare(_T("9F2601"))) {

		if(strRelplace.IsEmpty()) return FALSE;
		strField22 = strRelplace;

		strTag = _T("9F26");

		pos = strAPDU2.Find(_T("80AE800000"));
		if (pos > 0)
		{
			pos = strAPDU2.Find(strTag);
			if(pos>0) {
				//TO DO
				lenField = GetTagFieldByteLength(strAPDU2, strTag);
				AdjustInfo(strField22, lenField);
				ReplaceSubStrings(strAPDU, strField22, pos, strTag.GetLength()+2);	//2 is length

				return TRUE;
			}
		}
	}
	else if( !strTag.Compare(_T("9F2602"))) {

		if(strRelplace.IsEmpty()) return FALSE;
		strField22 = strRelplace;

		strTag = _T("9F26");

		pos = strAPDU2.Find(_T("80AE400000"));
		if (pos > 0)
		{
			pos = strAPDU2.Find(strTag);
			if(pos>0) {
				//TO DO
				lenField = GetTagFieldByteLength(strAPDU2, strTag);
				AdjustInfo(strField22, lenField);
				ReplaceSubStrings(strAPDU, strField22, pos, strTag.GetLength()+2);	//2 is length

				return TRUE;
			}
		}
	}
	else if( !strTag.Compare(_T("9F2603"))) {

		if(strRelplace.IsEmpty()) return FALSE;
		strField22 = strRelplace;

		strTag = _T("9F26");

		pos = strAPDU2.Find(_T("80AE000000"));
		if (pos > 0)
		{
			pos = strAPDU2.Find(strTag);
			if(pos>0) {
				//TO DO
				lenField = GetTagFieldByteLength(strAPDU2, strTag);
				AdjustInfo(strField22, lenField);
				ReplaceSubStrings(strAPDU, strField22, pos, strTag.GetLength()+2);	//2 is length

				return TRUE;
			}
		}
	}
	return FALSE;
}

CString GetOriginalTrack1(CString& strAPDU)
{
	CString strAPDU2 = strAPDU;
	CString strTag = _T("9F1F");
	CString strTrack1 = _T("");
	int pos, lenField;

	strAPDU2.MakeUpper();
	pos = strAPDU2.Find(strTag);
	if(pos>0) {
		//TO DO
		lenField = GetTagFieldByteLength(strAPDU2, strTag);
		strTrack1 = strAPDU.Mid(pos+strTag.GetLength()+2, lenField*2);

		char pAscii[64] = {0,};
		ConvertHexStringToBytes((char*)(LPCTSTR)strTrack1,(BYTE*)pAscii);

		strTrack1.Format(_T("%s"),pAscii);
	}
	return strTrack1;
}

int  GetOriginalTrack2Info(CString& strAPDU, CString& strField1, CString& strField2)
{
	CString strAPDU2 = strAPDU;
	CString strTag = _T("57");
	CString strTemp;
	int pos, lenField=-1;

	strAPDU2.MakeUpper();

	pos = strAPDU2.Find(_T("00B20"));
	if(pos>0) {

		pos = FindOneByteTagPos(strAPDU2, _T("70"), strTag, 20);	//AAADBBB - length : 13
		if(pos>0) {

			strTemp = strAPDU2.Mid(pos+2,2);	//First field of AID length
			ConvertHexCharToInt((char*)(LPCTSTR)strTemp, &lenField);

			strTemp = strAPDU2.Mid(pos+4, lenField*2);
			pos = strTemp.Find(_T("D"));
			if(pos>0){
				strField1 = strTemp.Left(pos);
				strField2 = strTemp.Mid(pos+1);
			}
			else
				return -1;
		}
	}

	return lenField;
}

CString GetCountryCode(CString& strAPDU)
{
	CString strCountryCode;
	CString strAPDU2 = strAPDU;
	CString strTag = _T("5F28");
	CString strTemp;
	int pos, lenField=-1;

	strAPDU2.MakeUpper();

	pos = strAPDU2.Find(_T("00B20"));
	if(pos>0) {
		pos = strAPDU2.Find(strTag, pos+2);	
		if(pos>0) {
			strTemp = strAPDU2.Mid(pos + strTag.GetLength(), 2);	//Get length
			ConvertHexCharToInt((char*)(LPCTSTR)strTemp, &lenField);
			strCountryCode = strAPDU2.Mid(pos + strTag.GetLength()+2, lenField*2);
		}
	}

	return strCountryCode;
}

CString GetExpireDate(CString& strAPDU)
{
	CString strExpDate;

	CString strAPDU2 = strAPDU;
	CString strTag = _T("5F24");
	CString strTemp;
	int pos, lenField=-1;

	strAPDU2.MakeUpper();

	pos = strAPDU2.Find(_T("00B20"));
	if(pos>0) {
		pos = strAPDU2.Find(strTag, pos+2);	
		if(pos>0) {
			strTemp = strAPDU2.Mid(pos + strTag.GetLength(), 2);	//Get length
			ConvertHexCharToInt((char*)(LPCTSTR)strTemp, &lenField);
			strExpDate = strAPDU2.Mid(pos + strTag.GetLength()+2, lenField*2);
		}
	}

	return strExpDate;
}

int DecideTrack2Length(int nTrackStringLength)
{
	//Length
	int nRealLen = nTrackStringLength;

	nTrackStringLength = nTrackStringLength/2;

	if(nTrackStringLength %2 != 0)
		nRealLen -=1;	//37 : 38 -> 19
	//32 : 32 -> 16
	//33 : 34 -> 17
	//27 : 28 -> 14	(?)
	//28 : 28 -> 14
	return nRealLen;

}

//In here makes whole scripts except autorun.
void MakeCommonSubScript(CStringArray& sarray, int nType)
{


	CString strTemp;

	//////////////////////////////////////////////////////////////////////////
	//	TRACE CARD
	if(nType == 0) {
		//Make Trace file -1
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));
		sarray.Add(_T("delete -AID 315041592E5359532E444446303101"));
		sarray.Add(_T("delete -AID 315041592E5359532E4444463031"));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strTracePath, sarray);
		handleCommands(sarray);

		sarray.RemoveAll();
	}
	else if(nType == 1) {
		//Make Trace file -2
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -scp 2 -scpimpl 0x15 -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));
		sarray.Add(_T("delete -AID 315041592E5359532E444446303101"));
		sarray.Add(_T("delete -AID 315041592E5359532E4444463031"));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strTracePath, sarray);
		handleCommands(sarray);
		sarray.RemoveAll();
	}

	//////////////////////////////////////////////////////////////////////////
	//		INSTALL CARD
	else if(nType == 2) {
		//Make Install card -1
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));

		//strTemp.Format(_T("install -file %s\\mac.dat -priv 4"),m_strWorkPath);
		strTemp.Format(_T("install -file mac.dat -priv 4"));
		sarray.Add(_T(strTemp));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strInstaladorPath, sarray);
		handleCommands(sarray);
		sarray.RemoveAll();
	}
	else if(nType == 3) {
		//Make Install card -2
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));

		//strTemp.Format(_T("install -file %s\\mac2.dat -priv 4"),m_strWorkPath);
		strTemp.Format(_T("install -file mac2.dat -priv 4"));
		sarray.Add(_T(strTemp));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strInstaladorPath, sarray);
		handleCommands(sarray);
		sarray.RemoveAll();
	}
	else if(nType == 4) {
		//Make Install card -3
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -scp 2 -scpimpl 0x15 -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));
		sarray.Add(_T("delete -AID 315041592E5359532E444446303101"));
		sarray.Add(_T("delete -AID 315041592E5359532E4444463031"));

		//strTemp.Format(_T("install -file %s\\mac.dat -priv 4"),m_strWorkPath);
		strTemp.Format(_T("install -file mac.dat -priv 4"));
		sarray.Add(_T(strTemp));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strInstaladorPath, sarray);
		handleCommands(sarray);
		sarray.RemoveAll();
	}
	else if(nType == 5) {
		//Make Install card -4
		sarray.Add(_T("mode_211"));
		sarray.Add(_T("enable_trace"));
		sarray.Add(_T("enable_timer"));
		sarray.Add(_T("establish_context"));
		sarray.Add(_T("card_connect"));
		sarray.Add(_T("select -AID a000000003000000"));
		sarray.Add(_T("open_sc -scp 2 -scpimpl 0x15 -security 1 -keyind 0 -keyver 0 -mac_key 404142434445464748494a4b4c4d4e4f -enc_key 404142434445464748494a4b4c4d4e4f"));
		sarray.Add(_T("delete -AID 315041592E5359532E444446303101"));
		sarray.Add(_T("delete -AID 315041592E5359532E4444463031"));

		//strTemp.Format(_T("install -file %s\\mac2.dat -priv 4"),m_strWorkPath);
		strTemp.Format(_T("install -file mac2.dat -priv 4"));
		sarray.Add(_T(strTemp));
		sarray.Add(_T("card_disconnect"));
		sarray.Add(_T("release_context"));

		//WriteStringFile(m_strInstaladorPath, sarray);
		handleCommands(sarray);
		sarray.RemoveAll();
	}


}

void WriteStringFile(CString& strFileName, CStringArray& sarray)
{



#if 0
	CStdioFile	file;
	CString strLine;
	char szCRL[3] = {0x0d, 0x0a,0};
	int n;

	if( !file.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite))
		return ;

	for( n=0;n<sarray.GetSize();n++) {
		strLine = sarray.GetAt(n);
		file.WriteString(strLine);
		file.WriteString(szCRL);
	}

	file.Close();
#endif

	handleCommands(sarray);


}