int main()
{
	// �����̸�, ũ��
	File* f1 = new File("a.txt", 10);	// ȭ���� �ڽ��� ũ�Ⱑ �ֽ��ϴ�.
	File* f2 = new Flie("b.txt", 20);
	File* f3 = new File("c.txt", 30);

	Folder* root = new Folder("ROOT");
	Folder* f01 = new Folder("A");
	Folder* f02 = new Folder("B");

	fo1->add(f1);
	fo2->add(f2);
	root->add(f3);
	root->add(fo1);
	root->add(fo2);

	cout << f1->getSize() << endl; // ������ ũ�⸦ ���Ҽ� �ֽ��ϴ�.
	cout << root->getSize() << endl; // ������ ũ�⸦ ���� �� �ֽ��ϴ�.
}
// ���հ�ü�� ������ü�� ���Ͻ� �ϴ� �Լ���?