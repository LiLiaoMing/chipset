#include "stdafx.h"
#include "SQLliteWrapper.h"

/*	example code to use

void test()
{

	vector<string> Row;
	Sqlite mySql("test.db");
	if(!mySql.ClassCheck())
	{
		return 1;
	}

	mySql.Exec("select * from address");

	int i;
	for(;;)
	{
		Row = mySql.GetRows();
		if(Row.size() > 0)
		{
			for(i = 0; i < Row.size(); i++)
			{
				cout << Row[i] << "  ";
			}
			cout << endl;
		}
		else
		{
			break;
		}
	}
	mySql.ExecFree();
}
*/

////////////////////////////////////////////////////////////////////////////////
// Prototypes for SQLite functions not included in SQLite DLL, but copied below
// from SQLite encode.c
////////////////////////////////////////////////////////////////////////////////
int sqlite3_encode_binary(const unsigned char *in, int n, unsigned char *out){
	int i, j, e, m;
	int cnt[256];
	if( n<=0 ){
		out[0] = 'x';
		out[1] = 0;
		return 1;
	}
	memset(cnt, 0, sizeof(cnt));
	for(i=n-1; i>=0; i--){ cnt[in[i]]++; }
	m = n;
	for(i=1; i<256; i++){
		int sum;
		if( i=='\'' ) continue;
		sum = cnt[i] + cnt[(i+1)&0xff] + cnt[(i+'\'')&0xff];
		if( sum<m ){
			m = sum;
			e = i;
			if( m==0 ) break;
		}
	}
	out[0] = e;
	j = 1;
	for(i=0; i<n; i++){
		int c = (in[i] - e)&0xff;
		if( c==0 ){
			out[j++] = 1;
			out[j++] = 1;
		}else if( c==1 ){
			out[j++] = 1;
			out[j++] = 2;
		}else if( c=='\'' ){
			out[j++] = 1;
			out[j++] = 3;
		}else{
			out[j++] = c;
		}
	}
	out[j] = 0;
	return j;
}

/*
** Decode the string "in" into binary data and write it into "out".
** This routine reverses the encoding created by sqlite3_encode_binary().
** The output will always be a few bytes less than the input.  The number
** of bytes of output is returned.  If the input is not a well-formed
** encoding, -1 is returned.
**
** The "in" and "out" parameters may point to the same buffer in order
** to decode a string in place.
*/
int sqlite3_decode_binary(const unsigned char *in, unsigned char *out){
	int i, c, e;
	e = *(in++);
	i = 0;
	while( (c = *(in++))!=0 ){
		if( c==1 ){
			c = *(in++);
			if( c==1 ){
				c = 0;
			}else if( c==2 ){
				c = 1;
			}else if( c==3 ){
				c = '\'';
			}else{
				return -1;
			}
		}
		out[i++] = (c + e)&0xff;
	}
	return i;
}


SQLliteWrapper::SQLliteWrapper(string adbname)
{
	int rc;
	classCheckFlag = false;
	mdbname = adbname;
	// DB 열기
	rc = sqlite3_open(mdbname.c_str(), &mdb);		//UTF-8
	//rc = sqlite3_open16(mdbname.c_str(), &mdb);		//UTF-16
	if (rc == SQLITE_OK)
	{
		classCheckFlag = true;
	}

	//mblobBuf = new char[MAX_BLOB_SIZE];
	mbloblen = 0;
	m_bBlobOK = false;
}


SQLliteWrapper::~SQLliteWrapper()
{
	sqlite3_close(mdb);
	//if (mblobBuf)
	//	delete[] mblobBuf;
}

bool SQLliteWrapper::ClassCheck()
{
	return classCheckFlag;
}


int SQLliteWrapper::Exec(string sql)
{
	int rc;

	// sql을 실행한다.
	rc = sqlite3_prepare(mdb, sql.c_str(), sql.length(), &stmt, NULL);	//UTF-8

	if (rc != SQLITE_OK)
	{
		return 1;
	}
	sqlite3_changes(mdb);
	return 0;
}

int SQLliteWrapper::execDML(const char* szSQL)
{
	if (!mdb)
		return 1;

	char* szError = 0;

	int nRet = sqlite3_exec(mdb, szSQL, 0, 0, &szError);

	if (nRet == SQLITE_OK)
	{
		return sqlite3_changes(mdb);
	}
	else
	{
		return 1;
	}
}

vector<string> SQLliteWrapper::GetRows()
{
	int rc;
	int n;
	int i;
	char buf[80];
	char *pBlob = NULL;
	mRow.clear();

	// sqlite3_step로 row 데이터를 하나씩 가져온다.
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE)
	{
		switch (rc)
		{
		case SQLITE_BUSY:
			break;
		case SQLITE_ERROR:
			break;
			// row 자료를 가져왔다면
			// 렬 자료를 읽어온다. 
			// 귀찮아서 렬 타입에 관계 없이 전부 string으로 넣어버렸다.
		case SQLITE_ROW:
			n = sqlite3_column_count(stmt);		//렬의 개수
			m_bBlobOK = false;
			for (i = 0; i < n; i++)
			{
				memset(buf, 0, sizeof(buf));

				switch (sqlite3_column_type(stmt, i))
				{
				case SQLITE_TEXT:
					mRow.push_back((const char *)sqlite3_column_text(stmt, i));
					break;
				case SQLITE_INTEGER:
					sprintf(buf, "%d", sqlite3_column_int(stmt, i));
					mRow.push_back(buf);
					break;
				case SQLITE_FLOAT:
					sprintf(buf, "%f", sqlite3_column_double(stmt, i));
					mRow.push_back(buf);
					break;
// 				case SQLITE_BLOB:
// 					pBlob = (char*)sqlite3_column_blob(stmt, i);
// 					n = sqlite3_column_bytes(stmt, i);		//바이트 수
// 					if (pBlob)
// 					{
// 						memcpy(mblobBuf, pBlob, n);
// 						mbloblen = n;
// 						m_bBlobOK = true;
// 					}
// 					//mRow.push_back(buf);
// 					break;
				case SQLITE_NULL:
					mRow.push_back("");
					break;
				default:
					//cout << "Unknown Field" << endl;
					break;
				}
			}
			break;
		case SQLITE_MISUSE:
			break;
		default:
			break;
		}
	}
	else
	{
		//cout << "SQL Exec Complete" << endl;
	}
	return mRow;
}

void SQLliteWrapper::ExecFree()
{
	sqlite3_finalize(stmt);
	mbloblen = 0;
}

int SQLliteWrapper::Exec(string sql, int(*callback)(void*, int, char**, char**))
{
	int rc;
	rc = sqlite3_exec(mdb, sql.c_str(), callback, 0, NULL);
	if (rc != SQLITE_OK)
	{
		return 1;
	}
	return 0;
}

bool SQLliteWrapper::IsDBEmpty()
{
	bool bRet = false;
	int rc;
	string sql = "select * from left";

	// sql을 실행한다.
	rc = sqlite3_prepare(mdb, sql.c_str(), sql.length(), &stmt, NULL);	//UTF-8

	if (rc != SQLITE_OK)
	{
		return 0;
	}
	
	sqlite3_changes(mdb);

	vector<string> Row;

	Row = GetRows();	//렬의 개수
	if (Row.size() <= 0)
	{
		bRet = true;
	}
	ExecFree();
	return bRet;
}

int getEncoded(LPBYTE pInput, int len)
{
	int bufLen = len*2;
	BYTE* ptmp = new BYTE[bufLen];

	memset(ptmp, 0, bufLen);
	memcpy(ptmp, pInput, len);
	int mnEncodedLen = sqlite3_encode_binary(ptmp, len, pInput);
	
	delete[] ptmp;
	return mnEncodedLen;
}

int getDecoded(LPBYTE pInput)
{
	int mnBinaryLen = sqlite3_decode_binary(pInput, pInput);
	
	return mnBinaryLen;
}