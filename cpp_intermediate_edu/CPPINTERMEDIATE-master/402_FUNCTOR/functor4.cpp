/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
#include <algorithm>
using namespace std;

// �Լ� ��ü�� �ڽŸ��� Ÿ���� �ִ�.
// signature�� ���� �ص� ��� �Լ���ü�� �ٸ� Ÿ���̴�.

struct Less
{
	inline bool operator()(int a, int b) const  {	return a < b; }
};

struct Greater
{
	inline bool operator()(int a, int b) const { return a > b;	}
};

//void Sort(int* x, int n, bool(*cmp)(int, int))

//void Sort(int* x, int n, Less cmp )

// ��å ���氡���ϰ� ��å�� �ζ��� ġȯ�Ǵ� �Լ�. ( ���ø� + �Լ� ��ü)
template<typename T> void Sort(int* x, int n, T cmp)
{
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (cmp(x[i], x[j]))
				swap(x[i], x[j]);
		}
	}
}
int main()
{
	int x[10] = { 1,3,5,7,9,2,4,6,8,10 };
	Less	f1; // Ÿ�� Less
	Greater f2; // Ÿ�� Greater
	Sort(x, 10, f1); // 
	Sort(x, 10, f2); // 
}