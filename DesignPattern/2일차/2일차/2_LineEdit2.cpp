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

// �ڽ� Ŭ������ ���ؼ� ��å�� �����Ѵ�.
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