/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

// inline1.cpp

	   int Add1(int a, int b) { return a + b; }
inline int Add2(int a, int b) { return a + b; }

int main()
{
	int n1 = Add1(1, 2); // ȣ��
	int n2 = Add2(1, 2); // ġȯ
						 // ���� : �ӵ��� ������
						 // ���� : �����ڵ尡 Ŀ����.

	// g++ inline1.cpp -S        => inline1.s
	// cl  inline1.cpp /FAs /Ob1 => inline1.asm
}

