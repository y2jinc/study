// 1. C스타일의 void* 기반 컨테이너

struct Node
{
	void* data;
	Node* next;
	Node(void* d, Node* n) : data(d), next(n) {}
};

// 싱글 리스트를 만들어 봅시다.
class slistImpl
{
	Node* head;
public:
	slistImpl() : head(0) {}
	// node 의 생성자를 잘 활용하면 간단하게 만들 수 있습니다.
	void push_front(void* a) { head = new Node(a, head); }

	void* front() { return head->data; }
};

// void* 기반 컨테이너를 직접 사용하면 항상 캐스팅을 해야 하므로 불편하다.
// 캐스팅을 책임지는 자식 클래스를 템플릿으로 제공하자.

// thin template 이라는 기술..
// (https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Thin_Template)
// 메모리가 부족한 모바일 환경에서 많이 쓰이는 기술.
template<typename T> class slist : public slistImpl
{
public:
	inline push_front(T a) { slistImpl::push_front((void*)a); }

	inline T	front() { return static_cast<T>(slistImpl::front()); }
};


int main()
{
	slist<int*> s;
	s.push_front(10);
	s.push_front(20);
	s.push_front(30); // 이때 메모리 그림을 생각해 보세요
}