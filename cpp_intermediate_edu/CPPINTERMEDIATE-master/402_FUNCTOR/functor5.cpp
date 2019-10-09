/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <algorithm>
using namespace std;

// �Ϲ� �Լ�
inline bool cmp1(int a, int b) { return a > b; }
inline bool cmp2(int a, int b) { return a < b; }

// functor
struct Less    { inline bool operator()(int a, int b) const { return a < b; }};
struct Greater { inline bool operator()(int a, int b) const { return a > b; }};

int main()
{
	int x[10] = { 1,3,5,7,9,2,4,6,8,10 };

	// STL sort : ��� ���ڰ� ���ø����� �Ǿ� �ִ�
	// 1. ����å���� �Ϲ��Լ��� �����Ҷ�.
	// ���� : ��å�� ��ü�ص� sort()����� �Ѱ��̴�. - �ڵ� �޸� ����
	// ���� : ��å �Լ��� �ζ��� ġȯ�ɼ� ����.
	sort(x, x + 10, cmp1); // sort( int*, int*, bool(*)(int, int)) �� �Լ� ����.
	sort(x, x + 10, cmp2); // sort( int*, int*, bool(*)(int, int)) �� �Լ� ����.

	// 2. ����å���� �Լ���ü �����Ҷ�.
	// ���� : ��å�Լ��� �ζ��� ġȯ�ɼ� �ִ� - ������.!
	// ���� : ��å�� ��ü�� Ƚ�� ��ŭ�� sort()���� ����.
	Less    f1;
	Greater f2;
	sort(x, x + 10, f1); // sort( int*, int*, Less) �� �Լ� ����.
	sort(x, x + 10, f2); // sort( int*, int*, Greater) �� �Լ� ����.
}