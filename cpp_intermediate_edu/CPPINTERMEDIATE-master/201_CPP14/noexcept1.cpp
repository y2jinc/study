#include <iostream>
using namespace std;
/*
// c++98
int foo()  // ���ܰ� �������� �ְ�, �������� �ִ�.
int foo() throw(int) // ���ܰ� �ִٴ� �ǹ�.
int foo() throw()    // ���ܰ� ���ٴ� �ǹ�.
{
	throw 1;
	return 0;
}
*/
// C++11
//void goo() noexcept(true) // ���ܰ� ����.
void goo() noexcept // ���� ����.
{
	throw 1;
}

void goo() 
{
	throw 1;
}

int main()
{
	goo();

	try
	{
		goo();
	}
	catch (...)
	{
		cout << "exception..." << endl;
	}
}




