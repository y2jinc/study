#include <iostream>
using namespace std;

// 2. �Լ� ������ ����� �̺�Ʈ ó�� : C# delegate
//								   ������ : Ÿ�پ׼� ����
//								   MFC : �޽�����.
//								   QT : single / slot
//							         �� ��� �̹��..
class MenuItem
{
	int id;

	void(Dialog::*handler)();
	Dialog* dlg;
public:
	MenuItem(int n) : id(n) {}

	virtual void command() 
	{
		handler(); // ��ϵ� �Լ� ȣ��. 
	}
};

int main()
{
	MenuItem m1(11), m2(22);
	m1.command();
}