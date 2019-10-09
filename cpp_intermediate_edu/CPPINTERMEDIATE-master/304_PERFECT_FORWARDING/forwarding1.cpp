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

// �Ϻ��� ���� : �����Լ��� ���ڸ� �޾Ƽ� ���� �Լ����� �Ϻ��ϰ�
//				�����ϴ� ����
// perfect forwarding.
template<typename F, typename A> 
void chronometry(F f, const A& arg)
{
	f(arg);
}

int main()
{
	int n = 0;
	//goo(n);
	//foo(5);
	chronometry(&goo, n); // goo(n)
	chronometry(&foo, 5); // foo(5) ����� ����ð�.

	cout << n << endl; // 30

}