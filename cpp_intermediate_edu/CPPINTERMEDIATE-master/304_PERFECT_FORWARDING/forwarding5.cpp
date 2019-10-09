/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

void foo(int  a)  { cout << "foo" << endl; }
void goo(int& a)  { cout << "goo" << endl; a = 30; }
void hoo(int&& a) { cout << "hoo" << endl; }

template<typename F, typename T>
void chronometry(F f, T&& arg)
{
	//f(static_cast<T&&>(arg));

	f(std::forward<T>(arg)); // forward()�� �ᱹ
							// ���������� ���� ĳ����
							// ����.
}

int main()
{
	int n = 0;

	chronometry(&foo, 5);
	chronometry(&goo, n);
	chronometry(&hoo, 10);

	cout << n << endl;
}




