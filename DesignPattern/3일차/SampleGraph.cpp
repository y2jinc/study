// SampleGraph.cpp
#include <iostream>
using namespace std;

// IGraph와 PieGraph 복사해 오세요 - 이름만 SampleGraph로 변경.

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

// SampleGraph 라는 이름은 DLL 을 만든 사라만 알고 있습니다.
// DLL을 사용하는 사람은 모르므로 객체는 DLL에서 만들어야 합니다.
// 객체를 만드는 함수를 약속해야 합니다.

extern "C" __declspec(dllexport) 	// DLL 만들때 꼭 필요한 내용
IGraph* CreateGraph()
{
	return new SampleGraph;
}

// cl SampleGraph.cpp /LD   ==> /LD 옵션이 DLL로 빌드해달라는 옵션
// SampleGraph.dll 이 생성되면
// "C:\\PlugIn 폴더를 하나 만드세요.. 그 아래 복사해두세요..