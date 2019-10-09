/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : new6.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

class Point
{
	int x, y;
public:
	Point() { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};

int main()
{
	// malloc : �޸𸮸� �Ҵ�
	Point* p1 = (Point*)malloc(sizeof(Point));


	// new : ��ü�� ���� ( �޸� �Ҵ� + ������ ȣ��)
	Point* p2 = new Point;


	Point* p3 = new Point;		// ���ο�޸𸮿� ��ü�� ������ �޶�
	Point* p4 = new(p1) Point;	// ���� �޸𸮿� ��ü�� ������ �޶�.
} 