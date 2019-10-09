/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/


int operator++(int& a, int)
{
	int temp = a;
	a = a + 1;
	return temp;
}
// ������ ���� ������ - ��������
int& operator++(int& a)
{
	a = a + 1;
	return a;
}

int main()
{
	int n = 0;
	n = 10;

	n++ = 20; // operator++(n, int) error.
	++n = 20; // operator++(n).   ok

	//++(++n);

	int a = 10, int b = 0;

	a + b = 20; // error.

	int x[3] = { 1,2,3 };
	x[0] = 5; // x.operator[](0) = 5;  lvalue
}