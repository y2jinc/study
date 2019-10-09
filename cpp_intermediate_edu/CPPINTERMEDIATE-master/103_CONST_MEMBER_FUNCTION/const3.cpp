/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : const3.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

// ��ü�� ���¸� �������� ���� ��� ����Լ���(getxxx)
// �ݵ�� const ��� �Լ��� �Ǿ�� �Ѵ�.
struct Rect
{
	int ox, oy, width, height;

	Rect(int x = 0, int y = 0, int w = 0, int h = 0) 
		: ox(x), oy(y), width(w), height(h) {}

	int getArea() const {	return width * height; }
};

void foo(  const Rect& r) // call by value ���ٴ� const & �� ����.
{
	int n = r.getArea(); // error.
}

int main()
{
	Rect r(0, 0, 10, 10);

	int n = r.getArea(); // ok.

	foo(r);
}