#include <iostream>
#include <vector>
using namespace std;

// 방법 1. 리스너 개념, java 나 안드로이드 앱에서 널리 사용

// 메뉴를 처리하고 싶은 클래스는 반드시 아래 인터페이스를 구현해야 한다.
struct IMenuListener
{
	virtual void onCommand(int id) = 0;
	virtual ~IMenuListener() {} // 부모의 소멸자는 가상 소멸자이어야한다. C++ 기본 문법
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
		// 메뉴가 선택된 사실을 외부에 알려야 한다.
		// 객체가 외부에 이벤트를 발생.
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
		cout << "id :" << id << " Camera 메뉴 선택됨" << endl;

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