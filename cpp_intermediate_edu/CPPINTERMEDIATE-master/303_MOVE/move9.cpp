/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <vector>
#include <type_traits>
#include "Test.h"		
using namespace std;

int main()
{
	Test* p1 = new Test[2];

	//Test* p2 = new Test[4];

	// 1. �ű� ���۴� �޸𸮸� �Ҵ� �Ѵ�.
	Test* p2 = static_cast<Test*>(
			operator new(sizeof(Test) * 4));

	for (int i = 0; i < 2; i++)
	{
		//p2[i] = move(p1[i]); // move ����..
		//new(&p2[i]) Test;		// ����Ʈ ������ ȣ��
		//new(&p2[i]) Test(p1[i]); // ���� ������
		//new(&p2[i]) Test(move(p1[i])); // move ������

		new(&p2[i]) Test(move_if_noexcept(p1[i])); 
	}

	// 2. ���ο� ��ü�� ����Ʈ ������ ȣ��
	for (int i = 2; i < 4; i++)
		new(&p2[i]) Test; // ����Ʈ ������ ȣ��.

}






















