#include <iostream>
using namespace std;

template<typename T> class Base
{
public:
	virtual void foo(const T a)
	{
		cout << "Base foo" << endl;
	}
};
class Derived : public Base<int*>
{
public:
	// ���� �Լ� foo�� �������ؼ�
	// "Derived foo" ��� ����� ������.
	// override ������� ����.
};

int main()
{
	Base<int*>* p = new Derived;
	p->foo(0);	// "Derived foo"
}




