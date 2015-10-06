#include "stdafx.h"
#include "charUtile.h"
#include <assert.h>

int AnsiToUTF8(char* szSrc, char* strDest, int destSize)
{
	WCHAR    szUnicode[2048];
	char    szUTF8code[2048];

	int nUnicodeSize = MultiByteToWideChar(CP_ACP, 0, szSrc, (int)strlen(szSrc), szUnicode, sizeof(szUnicode));
	int nUTF8codeSize = WideCharToMultiByte(CP_UTF8, 0, szUnicode, nUnicodeSize, szUTF8code, sizeof(szUTF8code), NULL, NULL);
	assert(destSize > nUTF8codeSize);
	memcpy(strDest, szUTF8code, nUTF8codeSize);
	strDest[nUTF8codeSize] = 0;
	return nUTF8codeSize;
}

int UTF8ToAnsi(char* szSrc, char* strDest, int destSize)
{
	WCHAR    szUnicode[2048];
	char    szAnsi[2048];

	int nSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, 0, 0);
	int nUnicodeSize = MultiByteToWideChar(CP_UTF8, 0, szSrc, -1, szUnicode, nSize);
	int nAnsiSize = WideCharToMultiByte(CP_ACP, 0, szUnicode, nUnicodeSize, szAnsi, sizeof(szAnsi), NULL, NULL);
	assert(destSize > nAnsiSize);
	memcpy(strDest, szAnsi, nAnsiSize);
	strDest[nAnsiSize] = 0;
	return nAnsiSize;
}

//std::string ===> CString
CString str2CString(string &str)
{
	CString ss;
	ss.Format(_T("%S"), str.c_str());
	return ss;
}

void UnicodeCString2Ansi(CString &str, char* pDst, int nDstLen)
{
	USES_CONVERSION;

#if _UNICODE
	char* pTmp = W2A(str);
#else
	char* pTmp = (char*)(LPCTSTR)str;
#endif

	int nLen = strnlen_s(pTmp, nDstLen-1);

	ZeroMemory(pDst, nDstLen);
	strcpy_s(pDst, nLen+1, pTmp);
}

void UnicodeCString2Float(CString &str, float &fVal)
{
	USES_CONVERSION;

#if _UNICODE
	char* pTmp = W2A(str);
#else
	char* pTmp = (char*)(LPCTSTR)str;
#endif

	fVal = (float)atof(pTmp);
}