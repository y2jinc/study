#include <iostream>
using namespace std;

// _ : ����� ��밡��.
// _�� �������� ������ : reserved

int operator""_k(unsigned long long v)
{
	return v * 1000;
}

int main()
{
	int n1 = 10_k; // operator""k(10)

	cout << n1 << endl; // 10000
}


