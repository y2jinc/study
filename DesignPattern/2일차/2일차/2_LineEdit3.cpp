#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// 변하는 것을 분리하는 2가지 기술
//  1. 가상함수로
//  2. 변하는 것을 다른 클래스로 => 교체 가능해야 한다. => 약한 결합이 필요

struct IValidator
{
	virtual bool validate(string s, char c) = 0;
	virtual bool iscomplete(string s) = 0;
};

// 주민 번호 : 801  1   확인

class LineEdit
{
	string data;
	IValidator* pVal;

public:
	LineEdit() : pVal(0) {}

	void setValidator(IValidator* p) { pVal = p; }

	string getData()
	{
		data.clear();

		while (1)
		{
			char c = _getch();
			if (c == 13 && (pVal == 0 || pVal->iscomplete(data))) 
				break;	// enter
			
			if (pVal == 0 || pVal->validate(data, c))
			{
				data.push_back(c);
				cout << c;
			}
		}
		cout << endl;
		return data;
	}
};

// 다양한 validation 정책을 제공하면 됩니다.
class LimitDigitValidator : public IValidator
{
	unsigned int value;
public:
	LimitDigitValidator(int n) : value(n) {}

	virtual bool validate(string s, char c)
	{
		return s.size() < value && isdigit(c);
	}

	virtual bool iscomplete(string s)
	{
		return s.size() == value;
	}
};

int main()
{
	LineEdit edit;
	LimitDigitValidator v(5);
	edit.setValidator(&v);

	while (1)
	{
		string s = edit.getData();
		cout << s << endl;
	}
}