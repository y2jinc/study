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
	int n = 0;

	cin >> n;  // 'a'

	if (cin.fail())
		cout << "����" << endl;

	if ( cin )  // if ( bool, ����, ������, �Ǽ� )
				// cin.operator void*() => C++98/03
				// cin.operator bool()  => C++11
		cout << "����" << endl;

}