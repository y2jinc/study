#include <iostream>
#include <type_traits>
using namespace std;

template<typename T> void copy_type(T* dst, T* arc, int sz)
{
	if (is_trivially_copyable<T>::value)
	{
		cout << "���� �����ڰ� trivial" << endl;
		memcpy(dst, src, sizeof(T)*sz);
	}
	else
	{
		cout << "���� �����ڰ� trivial ���� ���� ���" << endl;
		while (sz--)
		{
			new(dst) T(*src);
			++dst, ++src;
		}
	}
}