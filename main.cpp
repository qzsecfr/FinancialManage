#include <iostream>
#include "Command.h"

string help_info = "\
操作:\n\
\t用户管理类:\n\
\t\t登录 login $name$ $pswd$\n\
\t\t登出 logout\n\
\t\t修改密码 pswd $oldpswd$ $newpswd$\n\
\t\t修改用户名 name $newname$\n\
\t\t注销用户 deluser $pswd$\n\
\t交易类:\
\t\t收入 income $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\t支出 expense $y/m/d-h:m:s$ $amount$ $comment$\n\
\t\t删除交易 deltrans $no.$\n\
\t\t修改交易 modtrans $no.$ $y/m/d-h:m:s$ $type$ $amount$ $comment$[type:0-IN, 1-OUT]\n\
\t\t查看交易 acquire\n";

int main()
{
    Command* command = new Command();

    cout << "欢迎使用个人财务管理系统！" << endl;
    cout << "请输入命令或帮助（help）来获取帮助。输入exit或quit退出系统。" << endl;

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