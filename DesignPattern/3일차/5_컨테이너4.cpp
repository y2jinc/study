// 1. C��Ÿ���� void* ��� �����̳�

struct Node
{
	void* data;
	Node* next;
	Node(void* d, Node* n) : data(d), next(n) {}
};

// �̱� ����Ʈ�� ����� ���ô�.
class slistImpl
{
	Node* head;
public:
	slistImpl() : head(0) {}
	// node �� �����ڸ� �� Ȱ���ϸ� �����ϰ� ���� �� �ֽ��ϴ�.
	void push_front(void* a) { head = new Node(a, head); }

	void* front() { return head->data; }
};

// void* ��� �����̳ʸ� ���� ����ϸ� �׻� ĳ������ �ؾ� �ϹǷ� �����ϴ�.
// ĳ������ å������ �ڽ� Ŭ������ ���ø����� ��������.

// thin template �̶�� ���..
// (https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Thin_Template)
// �޸𸮰� ������ ����� ȯ�濡�� ���� ���̴� ���.
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
	s.push_front(30); // �̶� �޸� �׸��� ������ ������
}