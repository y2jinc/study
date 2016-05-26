#include <iostream>
#include <string>
#include <conio.h>
#include <vector>
using namespace std;

// ���հ�ü�� ������ü �� �ƴ϶� ���հ�ü �ڽŵ� �����Ѵ�.
// ���հ�ü�� ������ü�� ���� �θ� �־�� �Ѵ�.

// ���հ�ü�� ������ü�� ������ ���Ͻ� �Ǿ�� �Ѵ�.
// ���Ͻ��ϴ� �Լ�(command)�� �θ� �־�� �Ѵ�.

// "Composite" ���� : ���Կ� ���õ� ����

#define clrscr()	system("cls")

//��� �޴��� �θ� �޴�
class BaseMenu
{
	string title;
public:
	BaseMenu(string s) : title(s) {}

	virtual ~BaseMenu() {}

	string getTitle() { return title; }

	// ��� �޴��� ���� �ɼ� �ִ�... ������ �ڽ��� �ؾ��Ѵ�.
	virtual void command() = 0;
};

class MenuItem : public BaseMenu
{
	int	id;
public:
	MenuItem(string s, int n) : BaseMenu(s), id(n) {}

	virtual void command()
	{
		cout << getTitle() << " �޴��� ���õ�" << endl;
		_getch();
	}
};

class PopupMenu : public BaseMenu
{
	vector<BaseMenu*> v;	// �ٽ� !!!
	
public:
	PopupMenu(string s) : BaseMenu(s)
	{}

	void appendMenu(BaseMenu* p) { v.push_back(p); }

	virtual void command()
	{
		while (1)
		{
			clrscr();

			int sz = v.size(); // ���� �޴��� ����
			for (int i = 0; i < sz; ++i)
			{
				cout << i + 1 << ". " << v[i]->getTitle() << endl;
			}

			cout << sz + 1 << ". ���� �޴���" << endl;
			cout << "�޴��� �����ϼ��� >> ";
			int cmd;
			cin >> cmd;

			if (cmd < 1 || cmd > sz + 1)
				continue;

			if (cmd == sz + 1) // ���� �޴��� ������ ���
				return; // �Ǵ� break;

			// ������ �޴��� �����Ѵ�. ��� �ұ��?
			v[cmd-1]->command();	// ������...
		}
	}
};


int main()
{
	PopupMenu* menuBar = new PopupMenu("�޴���");
	PopupMenu* p1 = new PopupMenu("������");
	PopupMenu* p2 = new PopupMenu("Tape����");
	menuBar->appendMenu(p1);
	menuBar->appendMenu(p2);

	p1->appendMenu(new MenuItem("���߰�", 11));
	p1->appendMenu(new MenuItem("������", 12));

	p2->appendMenu(new MenuItem("Tape�߰�", 21));
	p2->appendMenu(new MenuItem("Tape����", 22));

	// �����Ϸ��� ??
	menuBar->command();
}