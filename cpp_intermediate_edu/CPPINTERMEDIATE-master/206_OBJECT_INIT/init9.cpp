/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

int main()
{
	int n1;		// default. �����Ⱚ
	int n2{};	// value.   0
	int n3();   // �Լ�����.

	int* p1 = new int;	// default. �����Ⱚ.
	int* p2 = new int();// value.   0
	int* p3 = new int{};// value.   0

	cout << *p1 << endl; // ������ ��
	cout << *p2 << endl; // 0
	cout << *p3 << endl; // 0
}