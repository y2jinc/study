// 5_컨테이너 - 91 page

// template 기반 컨테이너 (오브젝트 기반과 비교해보자)
// 장점 : 1. 타입 안정성이 뛰어남
//		 2. 객체뿐아니라 표준 타입을 저장할 수 있다.
//		 3. 꺼낼때(레퍼런싱) 항상 캐스팅이 불필요하다.
// 단점 : 템플릿이므로 코드 메모리 증가한다.

template<typename T> struct Node
{
	T data;
	Node* next;
	Node(T d, Node* n) : data(d), next(n) {}
};

// 싱글 리스트를 만들어 봅시다.
template<typename T> class slist
{
	Node<T>* head;
public:
	slist() : head(0) {}
	// node 의 생성자를 잘 활용하면 간단하게 만들 수 있습니다.
	void push_front(T a) { head = new Node<T>(a, head); }
};

int main()
{
	slist<int> s;
	s.push_front(10);
	s.push_front(20);
	s.push_front(30);
}