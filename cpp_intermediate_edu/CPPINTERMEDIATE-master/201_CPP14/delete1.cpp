#include <iostream>
using namespace std;

void foo(int n)
{
	cout << "foo(int)" << endl;
}

//void foo(double); // ����.

//void foo(double) = delete;

int main()
{
	foo(3.4); // foo(double)
}