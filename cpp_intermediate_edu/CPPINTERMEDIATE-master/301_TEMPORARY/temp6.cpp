/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : temp3.cpp
* Copyright (C) 2017 CODENURI Inc. All rights reserved.
*/
#include <iostream>
using namespace std;

struct Point
{
	int x, y;
	Point(int a = 0, int b = 0) { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};
// temp6.cpp

Point p; // ��������

Point foo() // ������ : �ӽð�ü�� ���ϵȴ�.
{
	return p;
}

Point& goo() // �������� : �ӽð�ü�� �������� �ʴ´�.
{
	return p;
}
int main()
{
	//Point ret = foo();

	foo().x = 10; // error
	goo().x = 20; // ok
	cout << p.x << endl;
}

