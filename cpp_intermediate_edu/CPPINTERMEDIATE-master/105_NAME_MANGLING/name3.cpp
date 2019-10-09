/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : name3.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <typeinfo>
using namespace std;

void foo(int a) 
{
}
// a.exe | c++filt -t
int main()
{
	void(*f1)(int) = &foo; // ok. �Լ� �ּ� ������
	void(*f2)(int) = foo;  // �Լ� �̸��� �Լ� �ּҷ� �Ͻ��� ����ȯ

	typedef void(*PF)(int); // �Լ� ������ Ÿ��
	typedef void F(int);    // �Լ� Ÿ��..

	cout << typeid(&foo).name() << endl; // void(*)(int)
	cout << typeid(foo).name() << endl;  // void(int)
}
