/*
* HOME       : ecourse.co.kr
* EMAIL      : smkang @ codenuri.co.kr
* COURSENAME : C++ Intermediate
* Copyright (C) 2018 CODENURI Inc. All rights reserved.
*/

class Test
{
	int value;
public:
	explicit Test(int n) : value(n) {}
};

int main()
{
	// �Ʒ� 2���� �������� ?
	
	Test t1(5);  // ���ڰ� �Ѱ��� ������ ȣ��
				 // direct initialization

	Test t2 = 5; // 1. ��ȯ �����ڸ� ����ؼ� 5�� ������ Test�� �ӽð�ü ����
				 // 2. �ӽð�ü�� ���� �����ڸ� ����ؼ� t2�� ����
				 // copy initialization
}
