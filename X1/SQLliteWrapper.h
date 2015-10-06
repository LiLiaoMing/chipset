#pragma once

#include <sqlite3.h>
#include <string>
#include <vector>

//#define MAX_IMG_SIZE		500*500*3
#define MAX_BLOB_SIZE		1024*1024
using namespace std;

class SQLliteWrapper
{
public:
	SQLliteWrapper();
	SQLliteWrapper(string adbname);
	~SQLliteWrapper();

	bool ClassCheck();
	int Exec(string sql);
	int execDML(const char* szSQL);
	vector<string> GetRows();
	void ExecFree();
	int Exec(string sql, int(*callback)(void*, int, char**, char**));
	char *GetBlobData();

	bool IsDBEmpty();

private:
	sqlite3 *mdb;
	string mdbname;
	sqlite3_stmt *stmt;
	vector<string> mRow;
	bool classCheckFlag;
	//char *mblobBuf;
	int  mbloblen;
	bool m_bBlobOK;

};

int getEncoded(LPBYTE pInput, int len);
int getDecoded(LPBYTE pInput);

