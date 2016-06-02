#include <iostream>
using namespace std;

class Dialog
{
public:
	void Close() { cout << "Dialog Close" << endl; }

};

void foo() { cout << "foo" << endl; }

// ---------------------------------------
struct ICommand
{
	virtual void Execute() = 0;
	virtual ~ICommand() {}
};

// 일반 함수 포인터를 관리하는 객체 클래스
class FunctionCommand : public ICommand
{
	typedef void(*HANDLER)();
	HANDLER handler;
public:
	FunctionCommand(HANDLER h) : handler(h) {}

	virtual void Execute() { handler(); }
};

// 멤버 함수 포인터를 관리하는 클래스
template<typename T> class MemberCommand : public ICommand
{
	typedef void(T::*HANDLER)();

	HANDLER handler;
	T*		target;

public:
	MemberCommand(HANDLER h, T* p) : handler(h), target(p) {}

	virtual void Execute() { (target->*handler(); }
};

//int main()
//{
//	FunctionCommand fc(&foo); // foo 주소를 관리하는 객체
//	fc.Execute(); // 결국 foo()
//
//	ICommand* p = &fc;
//
//}

// ---->

// 클래스 템플릿은 암시적 추론이 되지 않아서 항상 어려워 보입니다.
// 이럴때는 암시적 추론이 가능한 함수 템플릿을 도입하면 됩니다.

template<typename T> MemberCommand<T>* cmd(void(T::*f)(), T* p)
{
	return new MemberCommand<T>(f, p);
}

// FunctionCommand를 만들어 주는 cmd
FunctionCommand* cmd(void(*f)())
{
	return new FunctionCommand(f);
}

int main()
{
	Dialog dlg;

	ICommand* p1 = cmd(&foo);
	ICommand* p2 = cmd(&Dialog::Close, &dlg); // 이 줄과 아래 코드를 비교
	p1->Execute();
	p2->Execute();

	//MemberCommand<Dialog> mc(&Dialog::Close, &dlg);
	//mc.Execute();
}

// ---->
//template<typename T> T square(T a) { return a * a;  }
//// 다음중 쉬운 것은?
//square(3);		// 1 암시적 추론 -> 컴파일러가 결정.
//square<int>(3); // 2 명시적 추론 -> 사용자가 결정.
//
//list<int> s(10, 3); // 10개를 3으로 --> 클래스 템플릿은 암시적 추론이 되지 않는다.(***함수템플릿은 가능)