#include <iostream>
#include <vector>
using namespace std;

// 8. template method ���� : ������ ���� ��ü�帧�� �θ� �񰡻��Լ��� ����
//							 ���ؾ� �ϴ� �κи� �����Լ�ȭ �ؼ�
//							 �ڽ��� ������ �� �� �ְ� �ϴ� ����

// ��ü���� ������ ���� �θ� ���Ǵ� ����...

class Shape
{
public:
	// ������ ���� ��ü ���� �帧���� ���ؾ� �ϴ� �κ��� ã�Ƽ�
	// �����Լ��� �и��Ѵ�.

	virtual void DrawImp() 
	{
		cout << "Shape Draw" << endl;
	}

	void Draw() 
	{ 
		cout << "mutex lock" << endl;
		DrawImp();
		cout << "mutex unlock" << endl;
	}

	// �ڽ��� ���纻�� ����� �����Լ��� ���� ��찡 ����.
	virtual Shape* Clone() { return new Shape(*this); }
};

class Rect : public Shape
{
public:
	virtual void DrawImp() { cout << "Rect Draw" << endl; }

	virtual Shape* Clone() { return new Rect(*this); }
};

class Circle : public Shape
{
public:
	virtual void DrawImp() { cout << "Circle Draw" << endl; }

	virtual Shape* Clone() { return new Circle(*this); }
};

class Triangle : public Shape
{
public:
	virtual void DrawImp() { cout << "Triangle Draw" << endl; }

	virtual Shape* Clone() { return new Triangle(*this); }
};

int main()
{
	vector<Shape*> v;

	while (1)
	{
		int cmd;
		cin >> cmd;

		if (cmd == 1)
			v.push_back(new Rect);
		else if (cmd == 2)
			v.push_back(new Circle);
		else if (cmd == 8)
		{
			cout << "���° ������ ���� �ұ�� >> ";
			int k;
			cin >> k;

			v.push_back(v[k]->Clone());	// ������

										// k ��° ������ ���纻�� ����� v�� �߰��Ѵ�.
										// ��� �ұ�� ? k��°�� �����?
		}
		else if (cmd == 9)
		{
			for (int i = 0; i < v.size(); ++i)
				v[i]->Draw();	// ������(polymorphism)
								// ���� �Լ� ȣ���� ��Ȳ(��ü)�� ���� �ٸ��Լ��� ȣ�� �ϴ°�
		}
	}

}