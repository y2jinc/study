/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

class Point
{
	int x, y;
public:
	Point(int a = 0, int b = 0) : x(a), y(b) {}

	//Point(Point p) // Point p = p1. �ᱹ ���� �����ڰ� ������ ȣ��.

	// Point(Point& p)	// lvalue ��ü�� ���ڷ� ������ �ִ�.
						// �Լ� ���ϰ����� ��ȯ�Ǵ� �ӽð�ü�� 
						// ������ ����.
	Point(const Point& p) // lvalue��ü�� rvalue ��ü�� ��� ������
						  // �ִ�.
	{
		// ��� ��� ����.
	}
};

Point foo()
{
	Point ret(0, 0);
	return ret;
}

int main()
{
	Point p1(1, 1);	// ������ ȣ��
	Point p2(p1);	// Point( Point )  ���� ������.

	Point p3( foo() );
}