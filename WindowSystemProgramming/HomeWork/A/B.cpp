// B.cpp �ҽ� �Դϴ�.
// ���� ���� ȭ�� �ڵ��� ������ ���Ͽ� "hello"��� write ���� ���� �ǰ� ���ּ���..

// ����
// 1. ���ڷ� �Ѱܹ��� ���� �ڵ��� ������ "hello" �� write �Ѵ�.

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