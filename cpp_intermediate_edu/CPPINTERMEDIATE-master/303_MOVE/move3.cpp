/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

class Test
{
	int* resource;
public:
	Test()  {}	// �ڿ��Ҵ�
	~Test() {}	// �ڿ�����
	
	// ���� ������ : �������� �Ǵ� �������
	// ���ڷ� lvalue �� rvalue �� ��� ������ �ִ�
	Test(const Test& t) { cout << "Copy" << endl; }

	// Move ������ : ������ ����(�ڿ� ����)
	// rvalue�� ���� ������ �ִ�.
	Test(Test&& t)      { cout << "Move" << endl; }
};
Test foo()
{
	Test t;
	return t;
}
int main()
{
	Test t1;
	Test t2 = t1;		// ���� ������
	//Test t3 = Test();	// ���� ������.  //move ������
	Test t4 = foo();
}