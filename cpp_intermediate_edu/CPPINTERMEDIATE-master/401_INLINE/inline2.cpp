/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

// inline2.cpp
// 1. �ζ��� �Լ� ������ ������ �ð� �����Ѵ�.
// 2. �ζ��� �Լ��� �Լ������Ϳ� ��Ƽ� ����ϸ� ȣ��ȴ�.

	   int Add1(int a, int b) { return a + b; }
inline int Add2(int a, int b) { return a + b; }

int main()
{
	int n1 = Add1(1, 2); // ȣ��
	int n2 = Add2(1, 2); // ġȯ
	
	int(*f)(int, int);

	f = &Add2;
	//if (������Է��� 1 �̸�)
	//	f = &Add1;

	int n3 = f(1, 2); // ȣ�� ? ġȯ ?
}

