/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/


template<typename T> void foo(T&& a) {}

template<typename T> class Test
{
public:
	void goo(T&& a) {} // forwarding reference �ƴ�..

	template<typename U> void hoo(U&& a) {} // forwarding reference ����.
};

int main()
{
	int n = 10;
	
	foo(n);	// ok
	foo(10);// ok

	Test<int> t1; // T�� int�� ����.  void goo(int&& a)
	t1.goo(n);	// error
	t1.goo(10); // ok

	Test<int&> t2; // T int& => void goo( int& && a) => void goo(int& )
	t2.goo(n); // ok
	t2.goo(10); // error
}