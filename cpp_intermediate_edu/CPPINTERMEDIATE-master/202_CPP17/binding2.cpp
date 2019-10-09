#include <iostream>
#include <set>
using namespace std;

int main()
{
	set<int> s;

	s.insert(10);
	//pair<set<int>::iterator, bool> ret = s.insert(10); // ����
	/*
	auto ret = s.insert(10); // ����

	if (ret.second == false)
	{
		cout << "����" << endl;
	}
	*/
	/*
	auto [it, success] = s.insert(10); // ����

	if ( success == false)
	{
		cout << "����" << endl;
	}
	*/

	if ( auto[it, success] = s.insert(10); success == false)
	{
		cout << "����" << endl;
	}

}