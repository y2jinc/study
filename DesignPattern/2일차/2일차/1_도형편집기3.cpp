#include <iostream>
#include <vector>
using namespace std;

// 8. template method 패턴 : 변하지 않은 전체흐름은 부모가 비가상함수로 제공
//							 변해야 하는 부분만 가상함수화 해서
//							 자식이 재정의 할 수 있게 하는 패턴

// 객체지향 패턴중 가장 널리 사용되는 패턴...

class Shape
{
public:
	// 변하지 않은 전체 적인 흐름에서 변해야 하는 부분을 찾아서
	// 가상함수로 분리한다.

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

	// 자신의 복사본을 만드는 가상함수는 편리한 경우가 많다.
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
			cout << "몇번째 도형을 복사 할까요 >> ";
			int k;
			cin >> k;

			v.push_back(v[k]->Clone());	// 다형성

										// k 번째 도형의 복사본을 만들어 v에 추가한다.
										// 어떻게 할까요 ? k번째가 뭘까요?
		}
		else if (cmd == 9)
		{
			for (int i = 0; i < v.size(); ++i)
				v[i]->Draw();	// 다형성(polymorphism)
								// 동일 함수 호출이 상황(객체)에 따라 다른함수를 호출 하는것
		}
	}

}