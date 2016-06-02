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

// �Ϲ� �Լ� �����͸� �����ϴ� ��ü Ŭ����
class FunctionCommand : public ICommand
{
	typedef void(*HANDLER)();
	HANDLER handler;
public:
	FunctionCommand(HANDLER h) : handler(h) {}

	virtual void Execute() { handler(); }
};

// ��� �Լ� �����͸� �����ϴ� Ŭ����
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
//	FunctionCommand fc(&foo); // foo �ּҸ� �����ϴ� ��ü
//	fc.Execute(); // �ᱹ foo()
//
//	ICommand* p = &fc;
//
//}

// ---->

// Ŭ���� ���ø��� �Ͻ��� �߷��� ���� �ʾƼ� �׻� ����� ���Դϴ�.
// �̷����� �Ͻ��� �߷��� ������ �Լ� ���ø��� �����ϸ� �˴ϴ�.

template<typename T> MemberCommand<T>* cmd(void(T::*f)(), T* p)
{
	return new MemberCommand<T>(f, p);
}

// FunctionCommand�� ����� �ִ� cmd
FunctionCommand* cmd(void(*f)())
{
	return new FunctionCommand(f);
}

int main()
{
	Dialog dlg;

	ICommand* p1 = cmd(&foo);
	ICommand* p2 = cmd(&Dialog::Close, &dlg); // �� �ٰ� �Ʒ� �ڵ带 ��
	p1->Execute();
	p2->Execute();

	//MemberCommand<Dialog> mc(&Dialog::Close, &dlg);
	//mc.Execute();
}

// ---->
//template<typename T> T square(T a) { return a * a;  }
//// ������ ���� ����?
//square(3);		// 1 �Ͻ��� �߷� -> �����Ϸ��� ����.
//square<int>(3); // 2 ����� �߷� -> ����ڰ� ����.
//
//list<int> s(10, 3); // 10���� 3���� --> Ŭ���� ���ø��� �Ͻ��� �߷��� ���� �ʴ´�.(***�Լ����ø��� ����)