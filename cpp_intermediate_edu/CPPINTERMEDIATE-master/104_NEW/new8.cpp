/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : new8.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> v(10, 0);
	v.resize(7);
	cout << v.size() << endl; // 7
	cout << v.capacity() << endl; // 10


	// DBConnect : �����ڿ��� DB ����.
	vector<DBConnect> v2(10);

	v2.resize(7); // �޸𸮴� �������� ������
				  // �پ�� ��ü�� �Ҹ��ڴ� ȣ���ؾ��Ѵ�
				
	v2.resize(8); // ���ο� ��ü�� ���� �޸𸮴� �ִ�
				  // ������ �����ڸ� ȣ���ؼ�
				  // �ٽ� DB ������ �ؾ� �Ѵ�.
}