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
public:
	Test() {}	
	~Test() {}	
	Test(const Test& t) { cout << "Copy" << endl; }
	Test(Test&& t)      { cout << "Move" << endl; }
};

int main()
{
	Test t1;
	Test t2 = t1;		// Copy 	
	Test t3 = Test();	// Move
	Test t4 = static_cast<Test&&>(t1);	// Move
						// ���簡 �ƴ� move �����ڸ� ȣ���� �޶�.
	Test t5 = move(t2); // move�� ���������� ��ó�� ĳ�����Ѵ�.
}










