#ifndef _CHARUTILE_H_
#define _CHARUTILE_H_

#include <string>

using namespace std;

/*
SQLite에서 우리글을 별도의 처리 없이 사용 하게 되면 2가지 문제가 있다.
1. SQLite 뷰에서 DB 파일을 열어 보면 우리글이 깨져서 나온다.
   물론 별도의 처리를 하지 않아도 응용프로그람에서는 우리글이 제대로 나온다.
2. SQL 질의를 할 때, 제대로 동작하지 않는다.
   우리글을 사용하는 방법은 3가지가 있다.
   1. ansi : 지금 보고 있는 바와 같은 완성형 우리글 코드를 그대로 넣는 방법.
      우에서 말한 문제점이 있다.
      DB 클라이언트에서 우리글이 깨진다.
      DB에 질의 하면 검색이 이상하다.
   2. UTF-8 : UTF-8로 엔코딩해서 넣고, 불러올때는 디코딩해서 보는 방법.
      SQLite에서 TEXT 자료 형으로 자료를 입력하면 기본적으로 UTF-8로 저장된다.
      영어는 UTF-8과 같기 때문에 별도를 처리를 할 필요가 없지만  우리글은 넣기 전에  UTF-8 타입으로 바꿔주고
      질의 후 결과를 다시 윈도우즈 타입의 완성형으로 바꿔준다.
      례제는 이 방법을 사용 할 것이다.
   3. 유니코드 : 유니코드 자체를 넣는 방법.
      유니코드는 코드는 wchar_t 형으로 저장, DB에서는 ~~~16 함수를 사용한다.
      두 번째 방법으로 우리글을 저장한다.
      SQLite TEXT 자료형은 기본적으로 UTF-8이다 .  우리는 SQLite로 자료를 넘겨 주기 전에 AnsiToUTF8()을 사용하여 UTF-8로 변환한다.
      질의를 해서 SQLite TEXT에서 우리 프로그람으로 자료를 받을 때는 UTF8ToAnsi()를 사용하여 Ansi()로 변환한다..

//Ansi에서 UTF-8로 변환
char szName[100], szSql[200];
AnsiToUTF8( "샘플", szName, 100 );
sprintf_s( szSql, 200, "INSERT INTO member ( name, age ) values ( '%s', 20 )", szName );
int rst = sqlite3_exec( pSQLite3, szSql, callback, 0, &szErrMsg);

//UTF-8에서 Ansi로 변환
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