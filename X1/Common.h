#pragma once

#define MAX_TAG_LEN		10	//5   x 2
#define MAX_BUF_LEN		512 //256 x 2

typedef struct _tagCardInfo
{
	int  level;
	BOOL bTag;
	BOOL bIst;	//ist item
	BYTE sfi;	//SFI
	char szLabal[MAX_BUF_LEN];
	char szTag[MAX_TAG_LEN];
	char szValue[MAX_BUF_LEN];

} CARD_INFO, *LPCARD_INFO;
typedef CTypedPtrList<CPtrList, LPCARD_INFO> CARDINFOARRAY;

typedef struct _tagDBField
{
	int  idx;
	char szCountryCode[8];
	char szBank[128];
	char szType[64];
	char szBin[7];
	BYTE length;
	char szExpire[12];
	char szCode[5];
	char szTrack2[40];
	char szTrack1[30];
}DB_INFO, *LPDB_INFO;
typedef CTypedPtrList<CPtrList, LPDB_INFO> DBINFOARRAY;

CString ConvertBytesToHexStringEx(int iLength, BYTE * pbInput, BOOL bDivider=FALSE);
void    ConvertHexCharToInt(char * pbInput, int *pOut);
void    ConvertHexCharToByte(char * pbInput, BYTE *pOut);
void    ConvertHexStringToBytes(char * pbInput, BYTE *pOut);

char* FindCountryInCountryList(char* pCountryCode);
char* FindCountryCodeInCountryList(char* pCountry);


BOOL CheckLabelValidate(CString& strVal);

BOOL ReplaceInfoEx(CString& strAPDU, CString strTag, CString& strRelplace);

CString GetOriginalTrack1(CString& strAPDU);
int		GetOriginalTrack2Info(CString& strAPDU, CString& strField1, CString& strField2);	//28,32, 37 mode ?
CString GetCountryCode(CString& strAPDU);	//Now don't used
CString GetExpireDate(CString& strAPDU);	//Now don't used

int DecideTrack2Length(int nTrackStringLength);


void MakeCommonSubScript(CStringArray& sarray, int nType);
void WriteStringFile(CString& strFileName, CStringArray& sarray);