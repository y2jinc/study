#include <iostream>
using namespace std;

int main()
{
	int n1 = 0b10; 
	int n2 = 1'000'000; // 0b1111'0000

	long long n3; // 64 ��Ʈ 

	int x[3] = { 1,2,3 };

	auto n4 = x[0]; // �캯�� �������� �º��� Ÿ�԰���
	decltype(n1) d2; // int d2;

	decltype((n1)) d3; // int& d3;
}

