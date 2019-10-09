/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <vector>
using namespace std;

/*
template<typename T, typename Ax = allocator<T>> class vector
{
	T* buff;
public:
	vector(size_t sz, T v = T() ) {} // 1
	vector(initializer_list<T> s) {} // 2
};
*/

int main()
{
	vector<int> v = { 1,2,3,4,5 }; // ok. ���� ����

	vector<int> v1(10, 3);  // 1��. 10���� 3���� �ʱ�ȭ
	vector<int> v2{ 10,3 }; // 2��, 2���� ��Ҹ� 10, 3���� �ʱ�ȭ

	cout << v1.size() << endl; // 10
	cout << v2.size() << endl; // 2
}