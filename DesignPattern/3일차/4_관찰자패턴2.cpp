#include <iostream>
#include <vector>
using namespace std;

// ��� �׷����� �������̽�
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