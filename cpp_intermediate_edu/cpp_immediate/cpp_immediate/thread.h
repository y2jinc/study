#pragma once
#include <iostream>
#include <windows.h>

using namespace std;

// 라이브러리 내부 클래스
class Thread
{
public:

	// 안드로이드의 thread 클래스가 이런식으로 되어있다.
	// 1. 아래 함수는 반드시 static 함수 이어야 합니다.
	// 2. 아래 함수는 this가 없다. 그래서 함수 인자로 this를 전달하는 기술.
	static DWORD _stdcall threadMain(void* p)
	{
		Thread* const self = static_cast<Thread*>(p);

		self->Main();	// 가상함수 호출
				// this->Main() ==> Main(this) 로 변해야 한다.
		return 0;
	}

	void run()
	{
		CreateThread(0, 0, Thread::threadMain, (void*)this, 0, 0);
	}

	virtual void Main() // void Main(Thread* const this) 
	{}
};


