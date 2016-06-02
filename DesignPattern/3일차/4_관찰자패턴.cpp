#include <iostream>
#include <vector>
using namespace std;

// 모든 그래프의 인터페이스
struct IGraph
{
	virtual void onUpdate(int n) = 0;
	virtual ~IGraph() {}
};

// 관찰의 대상
class Table
{
	vector<IGraph*> v;
	int data;
public:
	void attach(IGraph* p) { v.push_back(p); }
	void detach(IGraph* p) {}

	void notify(int data)
	{
		for (int i = 0; i < v.size(); ++i)
			v[i]->onUpdate(data);
	}

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

// 다양한 그래프를 설계한다.
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