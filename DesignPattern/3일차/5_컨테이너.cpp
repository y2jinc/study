// 5_�����̳� - 91 page

struct Node
{
	int data;
	Node* next;
	Node(int d, Node* n) : data(d), next(n) {}
};

// �̱� ����Ʈ�� ����� ���ô�.
class slist
{
	Node* head;
public:
	slist() : head(0) {}
	// node �� �����ڸ� �� Ȱ���ϸ� �����ϰ� ���� �� �ֽ��ϴ�.
	void push_front(int a) { head = new Node(a, head);  }
};

int main()
{
	slist s;
	s.push_front(10);
	s.push_front(20);
	s.push_front(30); // �̶� �޸� �׸��� ������ ������
}