#include <iostream>
#include <vector>
using namespace std;

// 5. OCP ��Ģ : ���߿� ���ο� ��Ұ� �߰� �Ǿ ���� �ڵ�� �����Ǹ� �ȵȴ�.
//				 �������� OCP�� �����ϴ� ���� ���� �ڵ� �̴�.

// 6. Prototype ������ ���� : ���� ��ü�� ���縦 ���� ���ο� ��ü ����
//				java �� ��� Ŭ������ cloneable()�Լ��� �ֽ��ϴ�.
//				������ Ŭ�������� copy()�Լ��� �ֽ��ϴ�.

// 7. �����丵 ��� �Դϴ�. ������ ������

//	"replace type code with polymorphism"

class Shape
{
public:
	virtual void Draw() { cout << "Shape Draw" << endl; }
	
	// �ڽ��� ���纻�� ����� �����Լ��� ���� ��찡 ����.
	virtual Shape* Clone() { return new Shape(*this); }
};

class Rect : public Shape
{
public:
	virtual void Draw() { cout << "Rect Draw" << endl; }

	virtual Shape* Clone() { return new Rect(*this); }
};

class Circle : public Shape
{
public:
	virtual void Draw() { cout << "Circle Draw" << endl; }

	virtual Shape* Clone() { return new Circle(*this); }
};

class Triangle : public Shape
{
public:
	virtual void Draw() { cout << "Triangle Draw" << endl; }

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