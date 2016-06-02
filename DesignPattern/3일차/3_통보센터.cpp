#include <iostream>
#include <functional>
#include <string>
#include <map>
#include <vector>
using namespace std;

void foo() { cout << "foo" << endl; }
void goo() { cout << "goo" << endl; }

class NotificationCenter
{
	typedef function<void()> HANDLER;
	map<string, vector<HANDLER>> notify_map;
	
public:
	void Register(string key, HANDLER handler)
	{
		notify_map[key].push_back(handler);
	}

	void PostNotificationWithName(string key)
	{
		vector<HANDLER>& v = notify_map[key];

		for (int i = 0; i < v.size(); i++)
		{
			v[i]();	// function 이므로 ()로 호출
		}
	}

	// global 통보센터의 개념을 추가해 봅시다.
	static NotificationCenter& defaultCenter()
	{
		static NotificationCenter center;
		return center;
	}
};

int main()
{

	NotificationCenter& nc = NotificationCenter::defaultCenter();
	// NotificationCenter nc;

	nc.Register("LOWBATTERY", &foo);
	nc.Register("LOWBATTERY", &goo);

	// 배터리 모듈 쪽에서 
	nc.PostNotificationWithName("LOWBATTERY");
}