/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include "Test.h"		
using namespace std;

// T&  : lvalue �� ������ �ִ�.
// T&& : lvalue �� rvalue�� ��� ������ �ִ�. 
// ���ڷ� lvalue ���� : T => Test&  T&& : Test& && => Test&
//        rvalue ���� : T => Test   T&& : Test &&  => Test&&
template<typename T> 
typename remove_reference<T>::type &&
xmove(T&& obj)
{
	// ��ǥ : rvalue�� ĳ����.
	//return static_cast<T&&>(obj);

	return static_cast<typename remove_reference<T>::type &&>(obj);
}

int main()
{
	Test t1;
	Test t2 = t1;		 // copy
	Test t3 = xmove(t1); // move
	Test t4 = xmove( Test() ); // move
}





















