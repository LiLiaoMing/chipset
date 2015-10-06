#pragma once

class CASN1
{
public:
	CASN1();
	~CASN1(void);

	BYTE	m_aTag[MAX_TAG_LEN];
	BYTE	m_aValue[MAX_BUF_LEN];

	void DoProcess(BYTE* data, int dataLen, int nBaseLevel);

	CARDINFOARRAY& GetArrayInfo() {return m_infoArray;};
	LPCARD_INFO GetItemFromArray(int idx);
	LPCARD_INFO FindItemInArray(char* chTag);
	LPCARD_INFO FindNextItemInArray(char* chTag, POSITION& nPos, BOOL bStart = TRUE);
	void RemoveAllArray();

private:
	BYTE	m_aTag2[MAX_TAG_LEN];
	BYTE	m_aValue2[MAX_BUF_LEN];

	int		m_nBaseLevel;
	int		m_nLevel;

	void AddItemToArray(int level, char* pszTag, int tagLen, char* szValue, int valueLen, BOOL bIstItem=FALSE);
	void Decode(BYTE* asn1, int& anPos, int anLength);
	void DecodeTLV(BYTE* asn1, int& pos, BYTE* tag, int& tagLen, BYTE* content, int& contentLen);
	void DecodeTLV(BYTE* asn1, int& pos, unsigned short tag, int& tagLen, BYTE* content, int& contentLen);

	CARDINFOARRAY m_infoArray;
};
