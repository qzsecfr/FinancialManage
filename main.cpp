#include <iostream>
#include "Command.h"

string help_info = "\
����:\n\
\t�û�������:\n\
\t\t��¼ login $name$ $pswd$\n\
\t\t�ǳ� logout\n\
\t\t�޸����� pswd $oldpswd$ $newpswd$\n\
\t\t�޸��û��� name $newname$\n\
\t\t�����û� newuser $name$ $pswd$\n\
\t\tע���û� deluser $pswd$\n\
\t������:\n\
\t\t���� income $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\t֧�� expense $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\tɾ������ deltrans $no.$\n\
\t\t�޸Ľ��� modtrans $no.$ $y/m/d-h:m:s$ $type$ $amount$ $comment$[type:0-IN, 1-OUT]\n\
\t\t�鿴���� acquire\n\
\t\t�鿴���� report\n";

extern DataStorage* g_dataStorage;
extern User* g_user;
extern Trans* g_trans;

int main()
{
    Command command;
    //g_dataStorage = getGlobalDataStorage();
    //g_user = getGlobalUser();
    //g_trans = getGlobalTrans();

    cout << "��ӭʹ�ø��˲������ϵͳ��" << endl;
    cout << "����������������help������ȡ����������exit��quit�˳�ϵͳ��" << endl;

    while (true)
    {
        string input;
        cout << ">> ";
        getline(std::cin, input);

        if (strcmp(input.c_str(), "help") == 0 || strcmp(input.c_str(), "h") == 0)
        {
            cout << help_info;
        }
        else if (strcmp(input.c_str(), "exit") == 0 || strcmp(input.c_str(), "quit") == 0)
        {
            break;
        }
        else
        {
            command.processCommand(input);
            int test = 0;
        }
    }

    if (g_dataStorage)
    {
        delete g_dataStorage;
        g_dataStorage = nullptr;
    }
    if (g_user)
    {
        delete g_user;
        g_user = nullptr;
    }
    if (g_trans)
    {
        delete g_trans;
        g_trans = nullptr;
    }
    return 1;
}