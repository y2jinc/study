// A.cpp

// ����
// 1. a.txt �� �ٸ� ���μ������� ��밡���ϵ��� ����.
// 2. B.exe ���μ��� ����. (Kernel Object �ڵ��� ��� �޵��� ��.)
// 3. B.exe ���μ����� �ı� �� �� ���� ��ٸ�.
// 4. a.txt �� ���� ��� --> hello �� ��µǾ�� ��.

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

