#ifndef _CHARUTILE_H_
#define _CHARUTILE_H_

#include <string>

using namespace std;

/*
SQLite���� �츮���� ������ ó�� ���� ��� �ϰ� �Ǹ� 2���� ������ �ִ�.
1. SQLite �信�� DB ������ ���� ���� �츮���� ������ ���´�.
   ���� ������ ó���� ���� �ʾƵ� �������α׶������� �츮���� ����� ���´�.
2. SQL ���Ǹ� �� ��, ����� �������� �ʴ´�.
   �츮���� ����ϴ� ����� 3������ �ִ�.
   1. ansi : ���� ���� �ִ� �ٿ� ���� �ϼ��� �츮�� �ڵ带 �״�� �ִ� ���.
      �쿡�� ���� �������� �ִ�.
      DB Ŭ���̾�Ʈ���� �츮���� ������.
      DB�� ���� �ϸ� �˻��� �̻��ϴ�.
   2. UTF-8 : UTF-8�� ���ڵ��ؼ� �ְ�, �ҷ��ö��� ���ڵ��ؼ� ���� ���.
      SQLite���� TEXT �ڷ� ������ �ڷḦ �Է��ϸ� �⺻������ UTF-8�� ����ȴ�.
      ����� UTF-8�� ���� ������ ������ ó���� �� �ʿ䰡 ������  �츮���� �ֱ� ����  UTF-8 Ÿ������ �ٲ��ְ�
      ���� �� ����� �ٽ� �������� Ÿ���� �ϼ������� �ٲ��ش�.
      ������ �� ����� ��� �� ���̴�.
   3. �����ڵ� : �����ڵ� ��ü�� �ִ� ���.
      �����ڵ�� �ڵ�� wchar_t ������ ����, DB������ ~~~16 �Լ��� ����Ѵ�.
      �� ��° ������� �츮���� �����Ѵ�.
      SQLite TEXT �ڷ����� �⺻������ UTF-8�̴� .  �츮�� SQLite�� �ڷḦ �Ѱ� �ֱ� ���� AnsiToUTF8()�� ����Ͽ� UTF-8�� ��ȯ�Ѵ�.
      ���Ǹ� �ؼ� SQLite TEXT���� �츮 ���α׶����� �ڷḦ ���� ���� UTF8ToAnsi()�� ����Ͽ� Ansi()�� ��ȯ�Ѵ�..

//Ansi���� UTF-8�� ��ȯ
char szName[100], szSql[200];
AnsiToUTF8( "����", szName, 100 );
sprintf_s( szSql, 200, "INSERT INTO member ( name, age ) values ( '%s', 20 )", szName );
int rst = sqlite3_exec( pSQLite3, szSql, callback, 0, &szErrMsg);

//UTF-8���� Ansi�� ��ȯ
char szTrace[300];
char szAnsi[300];
sprintf_s( szTrace, 300, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL" );
UTF8ToAnsi( szTrace, szAnsi, 300 );
*/


int AnsiToUTF8(char* szSrc, char* strDest, int destSize);
int UTF8ToAnsi(char* szSrc, char* strDest, int destSize);
CString str2CString(string &str);	//std::string ===> CString

void UnicodeCString2Ansi(CString &str, char* pDst, int nDstLen);
void UnicodeCString2Float(CString &str, float &fVal);

#endif