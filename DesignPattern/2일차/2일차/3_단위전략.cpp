// ���� ������ ����� ��Ƽ ������ ��å ��ü

// ���� : ����ȭ ������ �ٸ� Ŭ������ ��밡��.
//		  ����ð� ��ü�� ����
// ����  : �����Լ� ����̶� �������ϰ� �ִ�.
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

List<int> st; // ��������. ��Ƽ�����忡 �������� �ʴ�.

int main()
{
	st.push_front(0);
}