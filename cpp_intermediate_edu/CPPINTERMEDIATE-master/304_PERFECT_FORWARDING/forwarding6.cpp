/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

#include <iostream>
using namespace std;

int x = 10;
int& foo(int  a) { return x; }

// error
/*
template<typename F, typename T>
decltype( f(std::forward<T>(arg)) ) chronometry(F f, T&& arg)
{
	return f( std::forward<T>(arg) ); 
}
*/
// ok.
/*
template<typename F, typename T>
auto chronometry(F f, T&& arg) -> decltype(f(std::forward<T>(arg)))
{
	return f(std::forward<T>(arg));
}
*/
/*
// ������ ok.. ���� �Լ��� ������ �����ϸ� ����..
template<typename F, typename T>
auto chronometry(F f, T&& arg) 
{
	return f(std::forward<T>(arg));
}
*/

// ������ ok.. ���� �Լ��� ������ �����ص� ok..
template<typename F, typename T>
decltype(auto) chronometry(F f, T&& arg)
{
	return f(std::forward<T>(arg));
}

int main()
{
	int& ret = chronometry(&foo, 10);
	ret = 20;
	cout << x << endl;
}




