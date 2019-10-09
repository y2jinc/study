/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

void goo(int& a) { cout << "goo" << endl; a = 30; }
void foo(int  a) { cout << "foo" << endl; }
void hoo(int&& a) { cout << "hoo" << endl; }


/*
template<typename F> void chronometry(F f, int& arg)
{
	f(arg);
}
template<typename F> void chronometry(F f, int&& arg)
{
	f(static_cast<int&&>(arg));
}
*/
// int&  : ������ lvalue
// int&& : ������ rvalue
// T&    : ��� Ÿ�� lvalue
// T&    : ��� Ÿ�� lvalue�� rvalue�� ������ �ִ�.
// forwarding reference
template<typename F, typename T> 
void chronometry(F f, T&& arg)
{
	f(static_cast<T&&>(arg));
}

int main()
{
	int n = 0;

	chronometry(&goo, n);
	chronometry(&hoo, 10);

	chronometry(&foo, 5);
	cout << n << endl;
}