#include <iostream>
#include <type_traits>
using namespace std;

struct Point
{
	int x, y;

	Point() {} // ����ڰ� ������ ����.

	//Point() = default; // �����Ϸ��� ����.

//	Point(const Point& ) = default;
//	Point(int a, int b) : x(a), y(b) {}
};

int main()
{
	Point p1{};  

	cout << p1.x << endl; // default ������ - 0
						  // ����ڰ� ���� - garbage




	cout << is_trivially_constructible<Point>::value
		 << endl;
}
