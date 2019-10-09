/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : ctor10.cpp
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <memory>
using namespace std;

struct Resource
{
	Resource() { cout << "acquire Resource" << endl; }
	~Resource() { cout << "release Resource" << endl; }
};

// �ذ�å 1. Raw Pointer ��� ����Ʈ ������ ���

class Test
{
//	Resource* p;
	unique_ptr<Resource> p;
public:
	Test() : p(new Resource)
	{
		cout << "Test()" << endl;
		throw 1;
	}
	~Test()
	{
		//delete p;
		cout << "~Test()" << endl;
	}
};

int main()
{
	try
	{
		Test t;
	}
	catch (...)
	{
		cout << "���� �߻�" << endl;
	}
}

