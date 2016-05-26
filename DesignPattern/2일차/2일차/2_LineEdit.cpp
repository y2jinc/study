#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

class LineEdit
{
	string data;

public:
	string getData()
	{
		data.clear();

		while (1)
		{
			char c = _getch();
			if (c == 13) break;	// enter
			if (isdigit(c))
			{
				data.push_back(c);
				cout << c;
			}
		}
		cout << endl;
		return data;
	}
};

int main()
{
	LineEdit edit;
	while (1)
	{
		string s = edit.getData();
		cout << s << endl;
	}
}