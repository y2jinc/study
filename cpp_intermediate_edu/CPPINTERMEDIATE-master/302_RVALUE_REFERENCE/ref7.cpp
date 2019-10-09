/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/


void f1(int&  a) {} 
void f2(int&& a) {} 
template<typename T> void f3(T&  a) {} 
template<typename T> void f4(T&& a) {}

// int&  : int ���� lvalue ���� ����.
// int&& : int ���� rvalue ���� ����.
// T&    : ��� Ÿ���� lvalue ���� ����.(�Լ� ����)

// T&&   : ��� Ÿ���� lvalue �� rvalue ��� ���� ����.(�Լ� ����)
//		   "universal reference"  => "forwarding reference"
//			lvalue �� �����ϸ� foo(n)  => T : int&    T&& : int& && => int&
//          rvalue �� �����ϸ� foo(10) => T : int     T&& : int&&    





