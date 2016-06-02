#include <iostream>
#include <vector>
#include "ioacademy.h" // 1일차 또는 카페에서 받으세요.

using namespace std;


struct IGraph
{
	virtual void onUpdate(int n) = 0;
	virtual ~IGraph() {}
};

// Table의 data가 변경되어도 관찰자 패턴의 기본 로직은 변하지 않는다.
// 관찰자 패턴의 기본 로직을 제공하는 부모를 만들자

// 변하는 것 : 가상함수로 자식이 재정의 하게
// 변하지 않는 것 : 부모 클래스로 제공

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
		cout << Name << endl; // 찾은 dll 파일 이름
		void* addr = ioacademy::IoLoadModule(Name); // DLL Load

		typedef IGraph* (*F)();

		F f = (F)ioacademy::IoGetFunctionAddress(addr, "CreateGraph"); // DLL의 함수를 찾는다.

		IGraph* graph = f(); // CreateGraph() 결국 new SampleGraph

		Subject* pThis = static_cast<Subject*>(param);
		pThis->attach(graph);	// 1일차 this_call 소스 참고

		return 1; // 계속 찾으라는 의미
	}


	void attach(IGraph* p) { v.push_back(p); }
	void detach(IGraph* p) {}

	void notify(int data)
	{
		for (int i = 0; i < v.size(); ++i)
			v[i]->onUpdate(data);
	}
};

// 관찰의 대상
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