/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

int main()
{
	int n = 10;
	int& r = n;
	const int c = n;
	const int& cr = c;

	// auto : �� ���� ���
	auto a1 = n;	// int
	auto a2 = r;	// int, 
	auto a3 = c;	// int
	auto a4 = cr;	// int


	auto& a5 = n;	// auto : int   a5 : int&
	auto& a6 = r;	// auto : int   a6 : int&
	auto& a7 = c;   // auto : const int a7 : const int& 
	auto& a8 = cr;	// auto : const int a8 : const int&


	const char* s1 = "hello"; // s1 ��ü�� const �ƴ�
							  // s1�� ���󰡸� const
	auto a9 = s1;	// const char*

	const char* const s2 = "hello";
	auto a10 = s2;  // const char*
}