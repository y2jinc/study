// 단위전략

#include <iostream>
using namespace std;

// 전략 패턴을 사용한 멀티 스레드 정책 교체

// 템플릿 인자로 정책 클래스를 교체하는 기술

// 단점 : 실행 시간 교체 불가능
// 장점 : 가상함수가 아닌 인라인 치환이 가능한 기술

// 모든 정책 클래스는 지켜야 하는 규칙을 가지게 된다.

template<typename T, typename ThreadModel = NoLock> 
class List : public ThreadModel
{
public:
	void push_front(const T& a)
	{
		Lock();
		//........
		Unlock();
	}
};

// 스레드 정책을 담은 정책 클래스
class NoLock
{
public:
	inline void Lock() {}
	inline void Unlock() {}
};

class Win32MutexLock
{
public:
	inline void Lock() { cout << "Lock" << endl; }
	inline void Unlock() { cout << "Unlock" << endl; }
};

List<int, NoLock> st; // 전역변수. 멀티스레드에 안전하지 않다.
List<int, Win32MutexLock> st2;

int main()
{
	st.push_front(0);
}