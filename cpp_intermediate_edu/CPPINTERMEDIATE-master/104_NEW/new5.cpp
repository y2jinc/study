/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : new5.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

class Point
{
	int x, y;
public:
	Point()  { cout << "Point()" << endl; }
	~Point() { cout << "~Point()" << endl; }
};

// placement new : �����ڸ� ȣ���ϱ� ���� ���� new
// 
/*
void* operator new(size_t sz, void* p)
{
	return p;
}
*/
int main()
{
	Point p;	

	//new Point; // ���ڰ� 1���� operator new()ȣ��.

	new(&p) Point; // ���ڰ� 2���� operator new()ȣ��.
					// �޸� �Ҵ��� �ƴ� �����ڸ�
					// ȣ���ϴ� �ڵ�
	p.~Point(); 
}