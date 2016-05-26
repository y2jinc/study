#include <iostream>
#include <vector>
using namespace std;

// 5. OCP 원칙 : 나중에 새로운 요소가 추가 되어도 기존 코드는 수정되면 안된다.
//				 다형성은 OCP를 만족하는 아주 좋은 코드 이다.

// 6. Prototype 디자인 패턴 : 기존 객체의 복사를 통한 새로운 객체 생성
//				java 의 모든 클래스는 cloneable()함수가 있습니다.
//				아이폰 클래스에는 copy()함수가 있습니다.

// 7. 리팩토링 용어 입니다. 생각해 보세요

//	"replace type code with polymorphism"

class Shape
{
public:
	virtual void Draw() { cout << "Shape Draw" << endl; }
	
	// 자신의 복사본을 만드는 가상함수는 편리한 경우가 많다.
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