#include <iostream>
using namespace std;

int foo()
{
	return 0;
}

int main()
{
	// ���� ��Ÿ��.
	int ret = foo();

	if (ret == 0)
	{
	}

	// C++17 ��Ÿ��
	// if ( init ����; ���ǹ� )
	if (int ret = foo(); ret == 0)
	{
		cout << "ret is 0" << endl;
	}

	// switch ������ �ʱ�ȭ ������ �߰��Ҽ� �ֽ��ϴ�.
	switch (int n = foo(); n)
	{
	case 0: break;
	case 1: break;
	}
}











