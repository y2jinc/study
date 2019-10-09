/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/


// T&& : lvalue �� rvalue�� ��� ���� ����.
//	    lvalue �����ϸ� T�� lvalue reference �� ����
//      rvalue �����ϸ� T�� �� Ÿ������ ����..

template<typename T> void f4(T&& a) {}

int main()
{
	int n = 10;

	// ����ڰ� T�� Ÿ���� ��������� �����Ҷ�
	f4<int>(10 );	// f4(int&& a)   => rvalue �� ����
	f4<int&>(n );	// f4(int& && a) => f4(int& a) => lvalue �� ����
	f4<int&&>(10 ); // f4(int&& && a)=> f4(int&& a) => rvalue ���� 

	// T�� Ÿ���� ��������� �������� ������
	f4(n);	// ok. �����Ϸ��� T�� int& �� ����.
	f4(10);	// ok. �����Ϸ��� T�� int  �� ����. f4(T&& ) => f4(int && ) 
}

