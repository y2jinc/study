/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

void true_function() {}

class istream   // basic_istream
{
public:
	bool fail() { return false; }

	// ��� 1. bool �� ��ȯ - ����. shift  ������ ���ȴ�.
	//operator bool() { return fail() ? false : true; }

	// ��� 2. void* ���� ��ȯ - C++ 98/03
	// operator void*() { return fail() ? 0 : this; }

	// ��� 3. �Լ� �����ͷ��� ��ȯ.
	//typedef void(*F)();
	//operator F() { return fail() ? 0 : &true_function; }

	// ��� 4. ��� �Լ� �����ͷ��� ��ȯ. - Safe BOOL
	//          if() ���� ������ �ִ� side effect�� ���� ����..
	struct Dummy
	{
		void true_function() {}
	};
	typedef void(Dummy::*F)();
	operator F() { return fail() ? 0 : &Dummy::true_function; }

};
istream cin;

int main()
{
	int n = 0;
	if ( cin ) {}
//	cin << n;
//	delete cin;

//	void(*f)() = cin;
}
