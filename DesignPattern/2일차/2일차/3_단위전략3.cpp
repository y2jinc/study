// 메모리 할당기

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
		// 메모리 재할당이 필요 합니다. 어떻게 할까요?
		// malloc(), new, calloc(), HealAlloc(), brk()
		T* p = a.allocate(sz); // 할당

		a.deallocate(p); // 메모리 해지
	}
};