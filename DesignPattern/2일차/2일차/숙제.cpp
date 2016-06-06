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
	// 파일이름, 크기
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

	cout << f1->getSize() << endl; // 파일은 크기를 구할수 있습니다.
	cout << root->getSize() << endl; // 폴더도 크기를 구할 수 있습니다.
}
// 복합객체와 개별객체를 동일시 하는 함수는? getSize() 