/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : conv2.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

// Point => int : ��ȯ ������   p.operator int()
// int => Point : ��ȯ ������   Point(int)

class Point
{
	int x, y;
public:
	Point()				: x(0), y(0) {}
	Point(int a, int b) : x(a), y(b) {}

	// ���ڰ� �Ѱ��� ������ -  ��ȯ ������
	//							�ٸ� Ÿ���� Point�� ��ȯ �ǰ� �Ѵ�.
	Point(int a) : x(a), y(0) {}

	operator int() 	{ return x; }
};

int main()
{
	Point p1;
	Point p2(1, 1);

	int n = 3;
	Point p(1, 2);

	n = p; // Point => int   p.operator int()
	p = n; // int => Point   n.operator Point() �� ������ �ȴ�.
		   //		         ������, n�� ��������� Ÿ���� �ƴϴ�
}



















