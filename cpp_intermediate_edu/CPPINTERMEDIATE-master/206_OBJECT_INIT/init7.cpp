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
	// direct initialization
	int n1(0);
	int n2{ 0 };    

	// copy initialization
	int n3 = 0;
	int n4 = { 0 }; 

	//---------------------------
	int n5;		// defalut initialization. �����Ⱚ	
	int n6{};	// value initialization.   0���� �ʱ�ȭ

	cout << n5 << endl; // �����Ⱚ
	cout << n6 << endl; // 0

	int n7(); // ����. �Լ� ����. 

}