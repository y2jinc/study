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

template<typename F> void chronometry(F f, int& arg)
{
	f(arg);
}
template<typename F> void chronometry(F f, int&& arg)
{
	// int&& arg = 10
	//f(arg);
	// �ذ�å : lvalue�� arg�� rvalue�� �ٽ� ĳ�����Ѵ�
	f(static_cast<int&&>(arg));
}

int main()
{
	//hoo(10); // ok..
	chronometry(&hoo, 10); // error
	
	int&& arg = 10; // 10 �� rvalue,
					// arg�� lvalue �̴�.

	
	int n = 0;

	chronometry(&goo, n);
	chronometry(&foo, 5);

	cout << n << endl;

}