/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* MODULE     : ctor11.cpp
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

// �ذ�å 2. two-phase constructor

class Test
{
	Resource* p;
public:
	Test() : p(0)
	{
		// ���� ���ɼ��� �ִ� ��� �۾��� ���� �ʴ´�.
		// �����Լ� ȣ��()
	}

	// �ڿ� �Ҵ� �����Լ�
	void Construct()
	{
		p = new Resource;
		//cout << "Test()" << endl;
		// �����Լ� ȣ��()
		throw 1;
	}

	~Test()
	{
		delete p;
		cout << "~Test()" << endl;
	}
};


int main()
{
	try
	{
		Test t;
		t.Construct(); // �ʿ��� �ڿ� �Ҵ�.
	}
	catch (...)
	{
		cout << "���� �߻�" << endl;
	}
}

