// 5_�����̳� - 91 page

// template ��� �����̳� (������Ʈ ��ݰ� ���غ���)
// ���� : 1. Ÿ�� �������� �پ
//		 2. ��ü�Ӿƴ϶� ǥ�� Ÿ���� ������ �� �ִ�.
//		 3. ������(���۷���) �׻� ĳ������ ���ʿ��ϴ�.
// ���� : ���ø��̹Ƿ� �ڵ� �޸� �����Ѵ�.

template<typename T> struct Node
{
	T data;
	Node* next;
	Node(T d, Node* n) : data(d), next(n) {}
};

// �̱� ����Ʈ�� ����� ���ô�.
template<typename T> class slist
{
	Node<T>* head;
public:
	slist() : head(0) {}
	// node �� �����ڸ� �� Ȱ���ϸ� �����ϰ� ���� �� �ֽ��ϴ�.
	void push_front(T a) { head = new Node<T>(a, head); }
};

int main()
{
	slist<int> s;
	s.push_front(10);
	s.push_front(20);
	s.push_front(30);
}