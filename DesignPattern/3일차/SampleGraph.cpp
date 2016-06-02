// SampleGraph.cpp
#include <iostream>
using namespace std;

// IGraph�� PieGraph ������ ������ - �̸��� SampleGraph�� ����.

//struct IGraph
//{
//	virtual void onUpdate(int n) = 0;
//	virtual ~IGraph() {}
//};

#include "IGraph.h"

class SampleGraph : public IGraph
{
public:
	void onUpdate(int n)
	{
		cout << "**** SampleGraph ****" << endl;
		cout << "data : " << n << endl;
		cout << "******************" << endl;
	}
};

// SampleGraph ��� �̸��� DLL �� ���� ��� �˰� �ֽ��ϴ�.
// DLL�� ����ϴ� ����� �𸣹Ƿ� ��ü�� DLL���� ������ �մϴ�.
// ��ü�� ����� �Լ��� ����ؾ� �մϴ�.

extern "C" __declspec(dllexport) 	// DLL ���鶧 �� �ʿ��� ����
IGraph* CreateGraph()
{
	return new SampleGraph;
}

// cl SampleGraph.cpp /LD   ==> /LD �ɼ��� DLL�� �����ش޶�� �ɼ�
// SampleGraph.dll �� �����Ǹ�
// "C:\\PlugIn ������ �ϳ� ���弼��.. �� �Ʒ� �����صμ���..