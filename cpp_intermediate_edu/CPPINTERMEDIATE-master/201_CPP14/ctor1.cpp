#include <iostream>
using namespace std;

struct Point
{
	int x, y;
	
	//Point()				: x(0), y(0) {}

	Point() : Point(0,0)
	{
		// �ٸ� �����ڸ� ȣ���Ҽ� ������ ?
		//Point(0, 0);	// �ӽð�ü�� �����ϴ� ǥ��.

		//new(this) Point(0, 0);

	}
	Point(int a, int b) : x(a), y(b) {}
	//void init() {}
};

int main()
{
	Point p;

	cout << p.x << endl;
	cout << p.y << endl;
}