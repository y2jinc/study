/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : ctor8.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

// �����ڿ����� �����Լ��� �������� �ʴ´�.
struct Base
{
	Base() { goo(); }
	
//	void foo() { goo(); }
	virtual void goo() { cout << "Base::goo" << endl; }
};

struct Derived : public Base
{
	int x;

	Derived() : x(10) {}
	virtual void goo() { cout << "Derived::goo" << x << endl; }
};

int main()
{
	Derived d;
//	d.foo();
}







