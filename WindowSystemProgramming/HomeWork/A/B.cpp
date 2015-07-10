// B.cpp 소스 입니다.
// 전달 받은 화일 핸들을 가지고 파일에 "hello"라고 write 한후 종료 되게 해주세요..

// 설명
// 1. 인자로 넘겨받은 파일 핸들을 가지고 "hello" 를 write 한다.

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"

#ifdef UNICODE
#undef UNICODE
#undef _UNICODE
#endif

#include <windows.h>
#include <tchar.h>

int main(int argc, char* argv[])
{
	HANDLE hFile = (HANDLE)atoi(argv[1]);

	char buf[] = "hello";
	DWORD len = strlen(buf);
	WriteFile(hFile, buf, len, NULL, NULL);

	return 0;
}