/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

int main()
{
	int n = 10;

	// ��Ģ 1. not const lvalue reference �� lvalue �� ���� ����
	int& r1 = n;	// ok
	int& r2 = 10;	// error

	// ��Ģ 2. const lvalue reference �� lvalue �� rvalue�� ��� ���� ����
	const int& r3 = n;	// ok
	const int& r4 = 10;	// ok   

	//const Point& r = Point(1, 1);
	//r.x = 10;

	// ��Ģ 3. rvalue reference �� rvalue �� ����ų�� �ִ�. C++11 ����.
	int&& r1 = n;	// error
	int&& r2 = 10;	// ok.
}