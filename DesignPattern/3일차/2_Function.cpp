#include <iostream>
using namespace std;

class Dialog
{
public:
	void Close() { cout << "Dialog Close" << endl; }

};

void foo() { cout << "foo" << endl; }

// C++ ǥ�ؿ��� �������� �Լ� ������ ������ ��ü�� �ֽ��ϴ�.
#include <functional>

int main()
{
	function<void()> f = &foo;

	f();

	Dialog dlg;
	f = bind(&Dialog::Close, &dlg); // cmd(&Dialog::Close, &dlg)

	f(); // dlg.Close();
}