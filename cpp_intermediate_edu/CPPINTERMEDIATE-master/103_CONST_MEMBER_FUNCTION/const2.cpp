/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : const2.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include "Point.h"
using namespace std;

int main()
{
	const Point p(1, 1);

	p.x = 10;		// error
	p.set(10, 20);	// error
	p.print();		// error.
					// ȣ��ǰ� �Ϸ��� print()��
					// �ݵ�� �������Լ��� �Ǿ���Ѵ�.
}

// �����ü�� ��� ����Լ��� ȣ���Ҽ� �ִ�.