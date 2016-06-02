#include <iostream>
#include <vector>
using namespace std;

// ��� 1. ������ ����, java �� �ȵ���̵� �ۿ��� �θ� ���

// �޴��� ó���ϰ� ���� Ŭ������ �ݵ�� �Ʒ� �������̽��� �����ؾ� �Ѵ�.
struct IMenuListener
{
	virtual void onCommand(int id) = 0;
	virtual ~IMenuListener() {} // �θ��� �Ҹ��ڴ� ���� �Ҹ����̾���Ѵ�. C++ �⺻ ����
};

class MenuItem
{
	int id;
	vector<IMenuListener*> Listeners;
public:
	MenuItem(int n) : id(n) {}

	void AddListener(IMenuListener* p) { Listeners.push_back(p); }

	virtual void command() 
	{
		// �޴��� ���õ� ����� �ܺο� �˷��� �Ѵ�.
		// ��ü�� �ܺο� �̺�Ʈ�� �߻�.
		for (int i = 0; i < Listeners.size(); ++i)
		{
			Listeners[i]->onCommand(id);
		}
			

	}
};

//------------------------
class Camera : public IMenuListener
{
public:
	virtual void onCommand(int id)
	{
		cout << "id :" << id << " Camera �޴� ���õ�" << endl;

		switch (id)
		{

		}
	}
};

int main()
{
	Camera cam;
	MenuItem m1(11), m2(22);

	m1.AddListener(&cam);
	m2.AddListener(&cam);

	m1.command();
}