/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <cstring>
using namespace std;

class Cat
{
	char* name;
	int   age;
public:
	Cat(const char* n, int a) : age(a)
	{
		name = new char[strlen(n) + 1];
		strcpy(name, n);
	}
	~Cat() { delete[] name; }

	// ���� ����� ������ ���� ������
	Cat(const Cat& c) : age(c.age)
	{
		name = new char[strlen(c.name) + 1];
		strcpy(name, c.name);
	}
	// ������ ����(�ڿ�����)��  �̵�(move) ������
	// 
	Cat(Cat&& c) : age(c.age), name(c.name)
	{
		c.name = 0; // �ڿ� ����
	}
};

Cat foo()	// ������ : �ӽð�ü(rvalue)
{
	Cat cat("NABI", 2);
	return cat;	
}

int main()
{
	Cat c = foo(); // �ӽð�ü.

}

