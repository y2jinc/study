#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

// ���̺귯�� ���� Ŭ����
class Thread
{
public:

	// �ȵ���̵��� thread Ŭ������ �̷������� �Ǿ��ִ�.
	// 1. �Ʒ� �Լ��� �ݵ�� static �Լ� �̾�� �մϴ�.
	// 2. �Ʒ� �Լ��� this�� ����. �׷��� �Լ� ���ڷ� this�� �����ϴ� ���.
	static DWORD _stdcall threadMain(void* p)
	{
		Thread* const self = static_cast<Thread*>(p);

		self->Main();	// �����Լ� ȣ��
				// this->Main() ==> Main(this) �� ���ؾ� �Ѵ�.
		return 0;
	}

	void run()
	{
		CreateThread(0, 0, Thread::threadMain, (void*)this, 0, 0);
	}

	virtual void Main() // void Main(Thread* const this) 
	{}
};


