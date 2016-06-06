#include <iostream>
#include <vector>
using namespace std;


class Item
{
	string	m_Name;

public:
	Item(const string& name) : m_Name(name) {}
	virtual size_t getSize() = 0;
};

class File : public Item
{
	size_t	m_Size;

public:
	File(const string& fileName, size_t size) : Item(fileName), m_Size(size) {}

	virtual size_t getSize()
	{
		return m_Size;
	}

};


class Folder : public Item
{
	vector<Item*>	m_Items;

public:
	Folder(const string& Name) : Item(Name) {}

	void add(Item* item)
	{
		m_Items.push_back(item);
	}

	virtual size_t getSize() 
	{ 
		size_t TotalSize = 0;
		for (size_t i = 0; i < m_Items.size(); ++i)
		{
			TotalSize += m_Items[i]->getSize();
		}

		return TotalSize;
	}
};

int main()
{
	// �����̸�, ũ��
	File* f1 = new File("a.txt", 10);
	File* f2 = new File("b.txt", 20);
	File* f3 = new File("c.txt", 30);

	Folder* root = new Folder("ROOT");
	Folder* fo1 = new Folder("A");
	Folder* fo2 = new Folder("B");

	fo1->add(f1);
	fo2->add(f2);

	root->add(f3);
	root->add(fo1);
	root->add(fo2);

	cout << f1->getSize() << endl; // ������ ũ�⸦ ���Ҽ� �ֽ��ϴ�.
	cout << root->getSize() << endl; // ������ ũ�⸦ ���� �� �ֽ��ϴ�.
}
// ���հ�ü�� ������ü�� ���Ͻ� �ϴ� �Լ���? getSize() 