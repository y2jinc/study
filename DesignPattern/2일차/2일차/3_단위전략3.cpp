// �޸� �Ҵ��

template<typename T>
class allocator
{
public:
	T* allocator(int sz) { return new T[sz]; }
	void deallocator(T* p) { delete[] p; }
};


template<typename T, typename Allocator = allocator<T> > 
class vector
{
	Allocator a;
public:
	void resize(int sz)
	{
		// �޸� ���Ҵ��� �ʿ� �մϴ�. ��� �ұ��?
		// malloc(), new, calloc(), HealAlloc(), brk()
		T* p = a.allocate(sz); // �Ҵ�

		a.deallocate(p); // �޸� ����
	}
};