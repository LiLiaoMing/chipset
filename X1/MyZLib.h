#pragma once

#define BUF		1024*1024		//compress data size
#define	DBUF	BUF*2 +13	//uncompress data size

class CMyZLib
{
public:
	CMyZLib(void);
	~CMyZLib(void);

	int Comp(LPBYTE pIn, int len);		//Inflate
	int Decomp(LPBYTE pIn, int len);	//Deflate

public:
	LPBYTE	m_pData;
	//LPBYTE	m_pDData;

	//BYTE	m_pData[DBUF];
};
