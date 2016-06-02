// 1. object ��� �����̳�
//	  �ֻ��� �θ� �����ϸ� ��� ���� Ŭ���� ��������
// ���� : template �� �ƴϴ�.. �ڵ� �޸𸮰� �������� �ʴ´�.
// ���� : 1. Ÿ�� �������� ��������.
//		 2. ǥ�� Ÿ���� ������ �� ����. ǥ�� Ÿ���� ���� Ŭ������ �ʿ��ϴ�.
//		 3. ������(���۷���) �׻� ĳ������ �ʿ��ϴ�.

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

// �̱� ����Ʈ�� ����� ���ô�.
class slist
{
	Node* head;
public:
	slist() : head(0) {}
	// node �� �����ڸ� �� Ȱ���ϸ� �����ϰ� ���� �� �ֽ��ϴ�.
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