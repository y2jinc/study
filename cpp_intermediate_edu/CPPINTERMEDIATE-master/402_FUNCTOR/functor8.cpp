/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <string>
#include <bitset>	
using namespace std;

int main()
{
	bitset<10> bs;

	bs.reset(); // ��� ��Ҹ� 0
	bs.reset(4); // 4��° ��Ʈ�� 0����

	bs[2] = 1;
	bs[1].flip(); // 1��°�� 1
	// 0000000110

	string s = bs.to_string();
	unsigned long n = bs.to_ulong();

	cout << s << endl; // 0000000110
	cout << n << endl; // 6
}