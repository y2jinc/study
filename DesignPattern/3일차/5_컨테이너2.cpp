// 1. object 기반 컨테이너
//	  최상위 부모를 보관하면 모든 하위 클래스 보관가능
// 장점 : template 가 아니다.. 코드 메모리가 증가하지 않는다.
// 단점 : 1. 타입 안정성이 떨어진다.
//		 2. 표준 타입을 저장할 수 없다. 표준 타입의 래퍼 클래스가 필요하다.
//		 3. 꺼낼때(레퍼런싱) 항상 캐스팅이 필요하다.

class object {};
class Point : public object {};
class Dialog : public object {};
class Integer : public object {};

struct Node
{
	object* data;
	Node* next;
	Node(object* d, Node* n) : data(d), next(n) {}
};

// 싱글 리스트를 만들어 봅시다.
class slist
{
	Node* head;
public:
	slist() : head(0) {}
	// node 의 생성자를 잘 활용하면 간단하게 만들 수 있습니다.
	void push_front(object* a) { head = new Node(a, head); }

	object* front() { return head->data; }
};

int main()
{
	slist s;
	s.push_front(new Point);
	s.push_front(new Point);
	s.push_front(new Dialog);

	Dialog* p = s.front();

	s.push_front(10);

}