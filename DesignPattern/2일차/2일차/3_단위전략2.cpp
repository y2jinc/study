// ��������

#include <iostream>
using namespace std;

// ���� ������ ����� ��Ƽ ������ ��å ��ü

// ���ø� ���ڷ� ��å Ŭ������ ��ü�ϴ� ���

// ���� : ���� �ð� ��ü �Ұ���
// ���� : �����Լ��� �ƴ� �ζ��� ġȯ�� ������ ���

// ��� ��å Ŭ������ ���Ѿ� �ϴ� ��Ģ�� ������ �ȴ�.

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

// ������ ��å�� ���� ��å Ŭ����
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

List<int, NoLock> st; // ��������. ��Ƽ�����忡 �������� �ʴ�.
List<int, Win32MutexLock> st2;

int main()
{
	st.push_front(0);
}