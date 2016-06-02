#include <iostream>
using namespace std;

// 2. 함수 포인터 방식의 이벤트 처리 : C# delegate
//								   아이폰 : 타겟액션 패턴
//								   MFC : 메시지맵.
//								   QT : single / slot
//							         이 모두 이방식..
class MenuItem
{
	int id;

	void(Dialog::*handler)();
	Dialog* dlg;
public:
	MenuItem(int n) : id(n) {}

	virtual void command() 
	{
		handler(); // 등록된 함수 호출. 
	}
};

int main()
{
	MenuItem m1(11), m2(22);
	m1.command();
}