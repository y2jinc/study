// A.cpp

// 설명
// 1. a.txt 를 다른 프로세스에서 사용가능하도록 생성.
// 2. B.exe 프로세스 생성. (Kernel Object 핸들을 상속 받도록 함.)
// 3. B.exe 프로세스가 파괴 될 때 까지 기다림.
// 4. a.txt 의 내용 출력 --> hello 가 출력되어야 함.

#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"

#ifdef UNICODE
#undef UNICODE
#undef _UNICODE
#endif

#include <windows.h>
#include <tchar.h>

int main()
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = { sizeof(si) };

	HANDLE hFile = CreateFile("a.txt", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	SetHandleInformation(hFile, HANDLE_FLAG_INHERIT, 
		HANDLE_FLAG_INHERIT);

	char s[32] = {0,};
	sprintf(s, "B.exe %d", hFile);

	BOOL b = CreateProcessA(0, s, 0, 0, TRUE,
		CREATE_NO_WINDOW,
		0, 0, &si, &pi);

	if (b)
	{
		WaitForSingleObject(pi.hProcess, INFINITE);	

		SetFilePointer(hFile, 0, 0, FILE_BEGIN);

		char buf[4096] = { 0 };
		DWORD len = 0;
		ReadFile(hFile, buf, 4096, &len, 0);

		printf("%s", buf);
		MessageBox(NULL, buf, "a.txt", MB_OK);

		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}

	return 0;
}

