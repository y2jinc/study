/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

int main()
{
	int  n = 0;
	int* p = &n;


	decltype(n) d1;	// int   // n = 10;
	decltype(p) d2;	// int*

					// (����) : ������ lvalue��� ����, �ƴϸ� �� Ÿ��
	decltype(*p)  d3; // *p = 10;   int&
	decltype((n)) d4; // (n) = 10;  int&

	decltype(n + n) d5; // n+n = 10 �� �ɼ� ����.  int 
	decltype(++n)   d6; // ++n = 10; ok..     int&
	decltype(n++)   d7; // n++ = 10; error.   int

	int x[3] = { 1,2,3 };

	decltype(x[0]) d8;  // x[0] = 10; ok    int&
	auto a1 = x[0];		// int
}