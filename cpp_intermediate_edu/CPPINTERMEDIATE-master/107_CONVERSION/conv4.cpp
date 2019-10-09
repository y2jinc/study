/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : conv4.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

class OFile
{
	FILE* file;
public:
	// explicit ������ : ���ڰ� �Ѱ��� �����ڰ� �Ͻ��� ��ȯ��
	//					����ϴ� ���� ���´�.

	explicit OFile(const char* filename, const char* mode = "wt")
	{
		file = fopen(filename, mode);
	}
	~OFile() { fclose(file); }

	operator FILE*() { return file; }
};

void foo(OFile f) {}

int main()
{
	OFile f("a.txt");
	foo(f); // ok..

	//foo("hello"); // const char* => OFile �� �Ͻ��� ��ȯ �߻�.
				  // ��ȯ ������
	//foo(static_cast<OFile>("hello"));

}


















