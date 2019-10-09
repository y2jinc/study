/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <initializer_list>
using namespace std;
class Point
{
	int x, y;
public:
	Point(int a, int b)           { cout << "int, int" << endl; }				// 1
	Point(initializer_list<int> ) { cout << "initializer_list<int>" << endl; }	// 2
};

int main()
{
	// Point p1(1, 1);	// 1��, ���ٸ� error
		
	//Point p2( { 1, 1 } );  // 2��. {1,1} ��ȯ�����ڸ� ����ؼ� �ӽð�ü����. ��������ڸ� ����ؼ� p2����
							// ������ error����, explicit �� �ƴҰ��. ��ȯ ����.

	//Point p3{ 1,1 }; // 2. ������ 1��

	//Point p4 = { 1,1 }; // 2. ������ 1��.
	
	//Point p5(1, 2, 3); // error
	
	//Point p6{ 1,2,3 }; // 2��.
	
	Point p7 = { 1,2,3 };

	vector<int> v = { 1,2,3,4 };
	
}
