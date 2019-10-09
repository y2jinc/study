/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : new7.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

class Point
{
	int x, y;
public:
	Point(int a, int b) : x(a), y(b) 
	{
		cout << "Point(int, int)" << endl;
	} 
};

int main()
{
	// Point ��ü�� ���� �Ѱ� ����� �ʹ�.
	Point* p1 = new Point(0, 0); // ok.


	// Point ��ü�� ���� 10�� ����� �ʹ�.
	//Point* p2 = new Point[10]; // error.

	// 1. �޸𸮸� ���� ���� �Ҵ�
	Point* p2 = static_cast<Point*>(operator new(sizeof(Point) * 10));
	
	// 2. �Ҵ��� �޸𸮿� ��ü�� ����(������ ȣ��)
	for (int i = 0; i < 10; i++)
		new(&p2[i]) Point(0,0);


	// 3. �Ҹ��� ȣ��
	for (int i = 9; i >= 0; i--)
		p2[i].~Point();

	// 4. �޸� ����.
	operator delete(p2);

	//vector<Point> v(10, Point(0,0));
}
