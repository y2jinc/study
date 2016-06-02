// 5_컨테이너 - 91 page

struct Node
{
	int data;
	Node* next;
	Node(int d, Node* n) : data(d), next(n) {}
};

// 싱글 리스트를 만들어 봅시다.
class slist
{
	Node* head;
public:
	slist() : head(0) {}
	// node 의 생성자를 잘 활용하면 간단하게 만들 수 있습니다.
	void push_front(int a) { head = new Node(a, head);  }
};

int main()
{
	slist s;
	s.push_front(10);
	s.push_front(20);
	s.push_front(30); // 이때 메모리 그림을 생각해 보세요
}