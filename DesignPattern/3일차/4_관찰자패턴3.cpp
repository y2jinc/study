#include <iostream>
#include <vector>
#include "ioacademy.h" // 1���� �Ǵ� ī�信�� ��������.

using namespace std;


struct IGraph
{
	virtual void onUpdate(int n) = 0;
	virtual ~IGraph() {}
};

// Table�� data�� ����Ǿ ������ ������ �⺻ ������ ������ �ʴ´�.
// ������ ������ �⺻ ������ �����ϴ� �θ� ������

// ���ϴ� �� : �����Լ��� �ڽ��� ������ �ϰ�
// ������ �ʴ� �� : �θ� Ŭ������ ����

class Subject
{
	vector<IGraph*> v;

public:
	Subject()
	{
		ioacademy::file::IoEnumFiles("c:\\PlugIn", "*.dll", LoadModule, this);
	}

	static int LoadModule(string Name, void* param)
	{
		cout << Name << endl; // ã�� dll ���� �̸�
		void* addr = ioacademy::IoLoadModule(Name); // DLL Load

		typedef IGraph* (*F)();

		F f = (F)ioacademy::IoGetFunctionAddress(addr, "CreateGraph"); // DLL�� �Լ��� ã�´�.

		IGraph* graph = f(); // CreateGraph() �ᱹ new SampleGraph

		Subject* pThis = static_cast<Subject*>(param);
		pThis->attach(graph);	// 1���� this_call �ҽ� ����

		return 1; // ��� ã����� �ǹ�
	}


	void attach(IGraph* p) { v.push_back(p); }
	void detach(IGraph* p) {}

	void notify(int data)
	{
		for (int i = 0; i < v.size(); ++i)
			v[i]->onUpdate(data);
	}
};

// ������ ���
class Table : public Subject
{
	int data;
public:

	void edit()
	{
		while (1)
		{
			cout << "data >> ";
			cin >> data;

			notify(data);
		}
	}
};

// �پ��� �׷����� �����Ѵ�.
class PieGraph : public IGraph
{
public:
	void onUpdate(int n)
	{
		cout << "**** PieGraph ****" << endl;
		cout << "data : " << n << endl;
		cout << "******************" << endl;
	}
};

class BarGraph : public IGraph
{
public:
	void onUpdate(int n)
	{
		cout << "**** BarGraph ****" << endl;
		cout << "data : " << n << endl;
		cout << "******************" << endl;
	}
};

int main()
{
	Table table;
	PieGraph pg; table.attach(&pg);
	BarGraph bg; table.attach(&bg);

	table.edit();
}