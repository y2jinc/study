/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

inline bool cmp1(int a, int b) { return a > b; }
inline bool cmp2(int a, int b) { return a < b; }
struct Less		{ inline bool operator()(int a, int b) const { return a < b; } };
struct Greater	{ inline bool operator()(int a, int b) const { return a > b; } };

template<typename T> void foo(T f)
{
	bool b = f(1, 2);
}
int main()
{
	foo(cmp1);
	foo(cmp2);
	foo(Less());
	foo(Greater());
}
// ���� 1. ������ ������ foo �Լ��� � �����ɱ�� ?
// ���� 2. ������ foo �Լ����� f(1,2) �� �ζ��� ġȯ ���δ� ?
// ���� 3. ������������ foo�� ������ �Լ��� ������ ?


