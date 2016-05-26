#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// ���ϴ� ���� �и��ϴ� 2���� ���
//  1. �����Լ���
//  2. ���ϴ� ���� �ٸ� Ŭ������ => ��ü �����ؾ� �Ѵ�. => ���� ������ �ʿ�

struct IValidator
{
	virtual bool validate(string s, char c) = 0;
	virtual bool iscomplete(string s) = 0;
};

// �ֹ� ��ȣ : 801  1   Ȯ��

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

// �پ��� validation ��å�� �����ϸ� �˴ϴ�.
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