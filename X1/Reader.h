#pragma once

class CReader
{
public:
	CReader(void);
	~CReader(void);

	//Card Type
	enum {
		VISA=0,
		MASTER,
		CIRRUS,
		MAESTRO,
		MAESTROUK,
		AMERICANEXPRESS,
		AMERICANEXPRESS2,
		EXPRESSPAY,
		JCB
	};


	BOOL Run();
	CARDINFOARRAY& GetCardInfoArray() {return m_infoArray;}
	void RemoveAllArray();

	LPCARD_INFO FindItemInArray(char* chTag, int nTagIdx);

	void GenerateIst(CStringArray& array);

private:
	void AddItemToArray(int level, const char* pszValue, BOOL bIstItem=FALSE, BYTE sfi=0);
	void AddSubArray(CARDINFOARRAY& array);

	void    MakeAPDULine(char* szTag, char* szValue, POSITION nPos, CString& strAPDU);
	void	GetAID(char* szLabal);
	void	GetRecordNum(POSITION nNextPos);

	int		GetCardType(char* szAID);
	int		GetCardTypeEx();
	int		GetAIDNum();

	CString FindValue(char* szArray, char* szVal);

	void MakeVISA_IST(CStringArray& array);
	void MakeMASTER_IST(CStringArray& array);
	
	BOOL CardConnect();
	void CardDisConnect();
	BOOL TransmitAPDU(CString& strAPDU);

private:
	CARDINFOARRAY	m_infoArray;

	CString			m_strAID;
	CString			m_strMasterAID;

	int				m_nRecord;
	int				m_nSFI; //Short File Identifier
};
