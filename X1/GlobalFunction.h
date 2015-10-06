#pragma once

/************************************************************************/
/*						GPShell                                         */
/************************************************************************/
/* Constants */
#define BUFLEN 1024
#define FILENAMELEN 256
#define READERNAMELEN 256
#define AIDLEN 16
#define APDULEN 261
#define INSTPARAMLEN 32
#define DELIMITER _T(" \t\r\n,")
#define DDES_KEY_LEN 16
#define PLATFORM_MODE_OP_201 OP_201
#define PLATFORM_MODE_GP_211 GP_211
#define PASSPHRASELEN 64
#define AUTOREADER -1

/* Data Structures */
typedef struct _OptionStr
{
	BYTE keyIndex;
	BYTE keySetVersion;
	BYTE newKeySetVersion;
	BYTE key[DDES_KEY_LEN];
	BYTE mac_key[DDES_KEY_LEN];
	BYTE enc_key[DDES_KEY_LEN];
	BYTE kek_key[DDES_KEY_LEN];
	BYTE current_kek[DDES_KEY_LEN];
	BYTE securityLevel;
	BYTE AID[AIDLEN+1];
	DWORD AIDLen;
	BYTE sdAID[AIDLEN+1];
	DWORD sdAIDLen;
	BYTE pkgAID[AIDLEN+1];
	DWORD pkgAIDLen;
	BYTE instAID[AIDLEN+1];
	DWORD instAIDLen;
	BYTE APDU[APDULEN+1];
	DWORD APDULen;
	BYTE secureChannel;
	TCHAR reader[READERNAMELEN+1];
	DWORD readerNumber;
	DWORD protocol;
	DWORD nvCodeLimit;
	DWORD nvDataLimit;
	DWORD vDataLimit;
	TCHAR file[FILENAMELEN+1];
	char passPhrase[PASSPHRASELEN+1];
	BYTE instParam[INSTPARAMLEN+1];
	DWORD instParamLen;
	BYTE element;
	BYTE privilege;
	BYTE scp;
	BYTE scpImpl;
	BYTE identifier[2];
	BYTE keyDerivation;
} OptionStr;

extern BYTE		g_recvAPDU[258];
extern DWORD	g_recvAPDULen;

bool GetOneLineEx(CString& strLine, CStringArray& pSrcBuf);
//static int handleCommands(FILE *fd);
int handleCommands(CStringArray& pCmdLines);