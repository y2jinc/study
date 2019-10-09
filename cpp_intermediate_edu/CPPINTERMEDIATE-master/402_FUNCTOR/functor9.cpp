/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <bitset>
using namespace std;

// 0 ~ 9 ������ �ߺ����� ���� ������ �����ϴ� �Լ�.

// �Ϲ��Լ� : ���۸� �ְ� ���°� ����.
bitset<10> bs;
bitset<10> bs1;
void clear_random() { bs.reset(); }
int random()
{
	int v = -1;
	//static bitset<10> bs; // 10���� 0���� �ʱ�ȭ
	do
	{
		v = rand() % 10;
	} while (bs.test(v));

	bs.set(v);	
	return v;
}

int main()
{
	for ( int i = 0; i < 10; i++)
		cout << random() << " "; 
	random();
}
