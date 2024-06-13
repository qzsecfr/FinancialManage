#include <iostream>
#include "Command.h"

string help_info = "\
����:\n\
\t�û�������:\n\
\t\t��¼ login $name$ $pswd$\n\
\t\t�ǳ� logout\n\
\t\t�޸����� pswd $oldpswd$ $newpswd$\n\
\t\t�޸��û��� name $newname$\n\
\t\tע���û� deluser $pswd$\n\
\t������:\
\t\t���� income $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\t֧�� expense $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\tɾ������ deltrans $no.$\n\
\t\t�޸Ľ��� modtrans $no.$ $y/m/d-h:m:s$ $type$ $amount$ $comment$[type:0-IN, 1-OUT]\n\
\t\t�鿴���� acquire\n";

int main()
{
    Command* command = new Command();

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
            command->processCommand(input);
        }
    }
    delete command;
    return 1;
}