/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

void foo(int& a)       { cout << "int&" << endl; }			// 1
void foo(const int& a) { cout << "const int&" << endl; }	// 2
void foo(int&& a)      { cout << "int&&" << endl; }			// 3

int main()
{
	int n = 10;

	foo(n);	// 1��, 1���� ������ 2��
	foo(10);// 3��, 3���� ������ 2��

	int& r1 = n;
	foo(r1);	// 1��, ������ 2��

	int&& r2 = 10;	// 10�� rvalue, 10�� ����Ű�� �̸��ִ� 
					//				r2 ���������� lvalue �̴�.
	foo(r2);		// 1��. 

	foo(static_cast<int&&>(r2));  // 3��.
}