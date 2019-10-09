#include <type_traits>
#include <set>
using namespace std;

// 1. type alias
using DWORD = int;


// 2. template alias
template<typename T, typename U>
using Duo = pair<T, U>;

Duo<int, double> d1; // pair<int, double>

template<typename T> 
using Ptr = T*;

Ptr<int> p2; // int*


// 3. 
template<typename T>
using Point = pair<T, T>;

Point<int> p;   // pair<int, int>


// 4. 

template<typename T>
using remove_pointer_t =
			typename remove_pointer<T>::type;

template<typename T> void foo(T a)
{
	// T���� �����͸� ������ Ÿ���� ���ϰ� �ʹ�.
	//typename remove_pointer<T>::type t; // C++11

	remove_pointer_t<T> t; // C++14 style
}



















