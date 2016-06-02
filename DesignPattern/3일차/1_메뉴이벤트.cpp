#include <iostream>
using namespace std;

class MenuItem
{
	int id;
public:
	MenuItem(int n) : id(n) {}

	virtual void command() {}
};

int main()
{
	MenuItem m1(11), m2(22);
	m1.command();
}