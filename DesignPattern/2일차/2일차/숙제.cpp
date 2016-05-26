int main()
{
	// 파일이름, 크기
	File* f1 = new File("a.txt", 10);	// 화일은 자신의 크기가 있습니다.
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

	cout << f1->getSize() << endl; // 파일은 크기를 구할수 있습니다.
	cout << root->getSize() << endl; // 폴더도 크기를 구할 수 있습니다.
}
// 복합객체와 개별객체를 동일시 하는 함수는?