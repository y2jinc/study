/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <bitset>
using namespace std;


// �Ϲ��Լ� : ���۸� �ְ� ���°� ����.
// �Լ���ü(fuctor) : ���۰� ���°� �ִ�.
class Random
{
	bitset<10> bs;
public:
	Random() {	bs.reset(); // ��� ��Ʈ�� 0����
	}
	int operator()()
	{
		int v = -1;
		do
		{
			v = rand() % 10;
		} while (bs.test(v));

		bs.set(v);
		return v;
	}
};


int main()
{
	Random random;
	Random random1;
	for (int i = 0; i < 10; i++)
		cout << random() << " ";
	random();
}
