#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BaseMenu
{
    string title;
public:
    BaseMenu( string s) : title(s) {}
    string getTitle() const { return title;}

    virtual void command() = 0;
};

class MenuItem : public BaseMenu
{
    int id;
public:
    MenuItem(string s, int n) : BaseMenu(s),  id(n) {}

    virtual void command()
    {
        cout << getTitle() << endl;
        getchar();
    }
};


class PopupMenu : public BaseMenu
{
    vector<BaseMenu*> v;
public:
    PopupMenu( string s) : BaseMenu(s) {}

    void addMenu(BaseMenu* p) { v.push_back(p);}

    virtual void command()
    {
        while( 1 )
        {
            system("cls");

            int sz = v.size();

            for ( int i = 0; i < sz; i++)
            {
                cout << i + 1 << ". " << v[i]->getTitle() << endl;
            }

            cout << sz + 1 << ". ���� �޴���" << endl;

            //------------------------------
            int cmd;
            cout << "�޴��� �����ϼ��� >> ";
            cin >> cmd;

            if ( cmd < 1 || cmd > sz + 1 )
                continue;

            if ( cmd == sz + 1 )
                break;



            v[cmd-1]-> command();
        }

    }
};

int main()
{
    PopupMenu* menubar = new PopupMenu("mebuBar");
    PopupMenu* pm1 = new PopupMenu("ȭ�鼳��");
    PopupMenu* pm2 = new PopupMenu("�Ҹ�����");
    MenuItem*  m1  = new MenuItem("���� Ȯ��", 11);

    menubar->addMenu( pm1 );
    pm1->addMenu( pm2 );
    menubar->addMenu( m1 );

    pm1->addMenu( new MenuItem("AAAA", 20));
    pm1->addMenu( new MenuItem("�ػ󵵺���", 21));
    pm1->addMenu( new MenuItem("��� ����", 22));

    pm2->addMenu( new MenuItem("���� ����", 31));

    // �����Ϸ���
    menubar->command();

}














//
