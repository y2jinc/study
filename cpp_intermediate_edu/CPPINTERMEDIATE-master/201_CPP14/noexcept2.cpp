#include <iostream>
using namespace std;

void algo1()
{
	// �������� ���� ���ɼ��� �ִ�.
}

void algo2() noexcept  // ������
{
	// �������� ���ܰ� ������ �ʴ´�.
}

class Test
{
public:
	Test() noexcept {}
};

int main()
{
	bool b1 = noexcept(algo1()); // 0. ������
	bool b2 = noexcept(algo2()); // 1

	cout << b1 << ", " << b2 << endl;

	bool b3 = is_nothrow_constructible<Test>::value;
	cout << b3 << endl;

}




