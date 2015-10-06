#include "StdAfx.h"
#include "MyZLib.h"

CMyZLib::CMyZLib(void)
{
	m_pData = new BYTE[DBUF];
}

CMyZLib::~CMyZLib(void)
{
	delete[] m_pData;
}

int CMyZLib::Comp(LPBYTE pIn, int len)
{
	uLong deflate_size = DBUF;

	ZeroMemory(m_pData, DBUF);
	compress(m_pData,&deflate_size, pIn, len);

	return deflate_size;
}

int CMyZLib::Decomp(LPBYTE pIn, int len)
{
	uLong inflate_size = BUF;

	ZeroMemory(m_pData, DBUF);
	uncompress(m_pData,&inflate_size, pIn, len);

	m_pData[inflate_size]=0;

	return inflate_size;
}
