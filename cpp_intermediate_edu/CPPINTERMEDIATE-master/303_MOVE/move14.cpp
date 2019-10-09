/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <string>
#include "Test.h"
using namespace std;

class Buffer
{
	size_t sz;
//	int*   buf;
	string tag;
	Test   test;
public:
	Buffer(size_t s, string t)
		: sz(s), tag(t) {}
	
	// ����ڰ� ������ ������ �����Ϸ��� �Ʒ� �������.
	// ��������..
	Buffer(const Buffer& b) : sz(b.sz), tag(b.tag), test(b.test)
	{
	}
	
	/*
	// move ������ : ��� ����� move �� �ű⵵�� �ۼ��Ѵ�.
	Buffer(Buffer&& b) noexcept
		: sz(move(b.sz)), tag(move(b.tag)), test(move(b.test))
	{
	}
	*/
	// ���Կ�����.
	// move ���Կ�����.
};

int main()
{
	Buffer b1(1024, "SHARED");
	Buffer b2 = b1; // copy
	Buffer b3 = move(b1); // move
}