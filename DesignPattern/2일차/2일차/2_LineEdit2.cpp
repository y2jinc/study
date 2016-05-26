#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

class LineEdit
{
	string data;

public:
	virtual bool validate(const char c) { return 0 != isdigit(c); }

	string getData()
	{
		data.clear();

		while (1)
		{
			char c = _getch();
			if (c == 13) break;	// enter
			if (validate(c))
			{
				data.push_back(c);
				cout << c;
			}
		}
		cout << endl;
		return data;
	}
};

// 자식 클래스를 통해서 정책을 변경한다.
class AddressEdit : public LineEdit
{
public:
	virtual bool validate(const char c) { return true; }
};

int main()
{
	AddressEdit edit;
	while (1)
	{
		string s = edit.getData();
		cout << s << endl;
	}
}