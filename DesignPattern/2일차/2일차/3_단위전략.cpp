// 전략 패턴을 사용한 멀티 스레드 정책 교체

// 장점 : 동기화 전략을 다른 클래스도 사용가능.
//		  실행시간 교체도 가능
// 단점  : 가상함수 기반이라서 성능저하가 있다.
struct ISync
{
	virtual void Lock() = 0;
	virtual void UnLock() = 0;
};

template<typename T> class List
{
	ISync* pSync;
public:
	void setSync(ISync* p) { pSync = p; }
	void push_front(const T& a)
	{
		pSync->Lock();
		//........
		pSync->UnLock();
	}
};

List<int> st; // 전역변수. 멀티스레드에 안전하지 않다.

int main()
{
	st.push_front(0);
}