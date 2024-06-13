#include "Command.h"

int Command::processCommand(string command)
{
    vector<string> args;
    if (!parseCommand(command, args))
    {
        cerr << "输入的命令无效！" << endl;
        return -1; // command error
    }

    return commandDistribute(args);
}

int Command::parseCommand(string command, vector<string>& args)
{
    args.clear();
    size_t start = 0;
    size_t end = command.length();
    bool flag = false;
    int i = 0;
    for (; i < command.length(); ++i)
    {
        if (command[i] != ' ' && !flag)
        {
            flag = true;
            start = i;
        }
        else if (command[i] == ' ')
        {
            if (flag)
            {
                end = i;
                args.push_back(command.substr(start, end - start));
                flag = false;
            }
        }
    }
    if (flag)
    {
        end = i;
        args.push_back(command.substr(start, end - start));
    }
    
    return 1;
}

COMTYPE Command::com2Type(string com)
{
    if (0 == strcmp(com.c_str(), "login")) return COMTYPE::LOGIN;
    if (0 == strcmp(com.c_str(), "logout")) return COMTYPE::LOGOUT;
    if (0 == strcmp(com.c_str(), "pswd")) return COMTYPE::PSWD;
    if (0 == strcmp(com.c_str(), "name")) return COMTYPE::NAME;
    if (0 == strcmp(com.c_str(), "deluser")) return COMTYPE::DELUSER;
    if (0 == strcmp(com.c_str(), "income")) return COMTYPE::INCOME;
    if (0 == strcmp(com.c_str(), "expense")) return COMTYPE::EXPENSE;
    if (0 == strcmp(com.c_str(), "deltrans")) return COMTYPE::DELTRANS;
    if (0 == strcmp(com.c_str(), "modtrans")) return COMTYPE::MODTRANS;
    if (0 == strcmp(com.c_str(), "acquire")) return COMTYPE::ACQUIRE;

    return COMTYPE::INVALID;
}

int Command::commandDistribute(const vector<string>& args)
{
    // Actions:
    //  User:
    //      login $name$ $pswd$
    //      logout
    //      pswd $oldpswd$ $newpswd$
    //      name $newname$
    //      deluser $pswd$
    //  Trans:
    //      income $y/m/d-h:m:s$ $amount$ $comment$
    //      expense $y/m/d-h:m:s$ $amount$ $comment$
    //      deltrans $no.$
    //      modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-IN, 1-OUT]
    //      acquire
    if (args.empty())
    {
        return -1;
    }

    COMTYPE type = com2Type(args[0]);
    switch (type)
    {
    case COMTYPE::LOGIN:
        return execLogin(args);
    case COMTYPE::LOGOUT:
        return execLogout(args);
    case COMTYPE::PSWD:
        return execChangePswd(args);
    case COMTYPE::NAME:
        return execChangeName(args);
    case COMTYPE::DELUSER:
        return execDelUser(args);
    case COMTYPE::INCOME:
        return execAddTrans(args, TransType::IN);
    case COMTYPE::EXPENSE:
        return execAddTrans(args, TransType::OUT);
    case COMTYPE::DELTRANS:
        return execDelTrans(args);
    case COMTYPE::MODTRANS:
        return execModTrans(args);
    case COMTYPE::ACQUIRE:
        return execAcquireTrans(args);
    default:
        break;
    }

    return 0;
}

int Command::execLogin(const vector<string>& args)
{
    // login $name$ $pswd$
    if (args.size() < 3)
    {
        cerr << "登录命令错误，应为：login $name$ $pswd$" << endl;
        return -1;
    }
    
    int ret = g_user->login(args[1], args[2]); // -1:no user, 0:fail, 1:success
    if (ret == -1)
    {
        cerr << "查无此用户！" << endl;
    }
    else if (ret == 0)
    {
        cerr << "密码错误，请重试！" << endl;
    }
    else
    {
        cout << "登陆成功！" << endl;
    }
    int uid = -1;
    string tmp;
    g_user->getUserInfo(uid, tmp);
    g_trans->setUid(uid);
    return ret;
}

int Command::execLogout(const vector<string>& args)
{
    int ret = g_user->logout();
    cout << "已登出！" << endl;
    g_trans->setUid(-1);
    return ret;
}

int Command::execChangePswd(const vector<string>& args)
{
    // pswd $oldpswd$ $newpswd$
    if (args.size() < 3)
    {
        cerr << "修改密码命令错误，应为：pswd $oldpswd$ $newpswd$" << endl;
        return -1;
    }

    int ret = g_user->changePswd(args[1], args[2]); // -2: not logged, -1: no user, 0: wrong pswd, 1: success
    if (ret == -2)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 0)
    {
        cerr << "原密码错误！" << endl;
    }
    else if (ret == 1)
    {
        cout << "密码修改成功！" << endl;
    }
    return ret;
}

int Command::execChangeName(const vector<string>& args) // -2: not logged, -1: no user, 1: success
{
    // name $newname$
    if (args.size() < 2)
    {
        cerr << "修改用户名命令错误，应为：name $newname$" << endl;
        return -1;
    }

    int ret = g_user->changeName(args[1]);
    if (ret == -2)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 1)
    {
        cout << "用户名修改成功！" << endl;
    }
    return ret;
}

int Command::execDelUser(const vector<string>& args) // -2: not logged, -1: no user, 1: success
{
    //  deluser $pswd$
    if (args.size() < 2)
    {
        cerr << "注销账户命令错误，应为：deluser $pswd$" << endl;
        return -1;
    }

    int ret = g_user->delUser(args[1]);
    if (ret == -2)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 1)
    {
        cout << "账户注销成功！" << endl;
    }
    return ret;
}

int Command::execAddTrans(const vector<string>& args, TransType transtype)
{
    // income $y/m/d-h:m:s$ $amount$ $comment$
    // expense $y/m/d-h:m:s$ $amount$ $comment$
    if (args.size() < 4)
    {
        cerr << "记账命令错误，应为：" << endl;
        cerr << "\t income $y/m/d-h:m:s$ $amount$ $comment$" << endl;
        cerr << "或" << endl;
        cerr << "\t expense $y/m/d-h:m:s$ $amount$ $comment$" << endl;
        return -1;
    }
    UTC utc;
    if (!string2UTC(args[1], utc))
    {
        cerr << "输入日期时间格式错误，应为：$y/m/d-h:m:s$" << endl;
        return 0;
    }

    Transaction trans;
    trans.type = transtype;
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[2].c_str());
    trans.comment = args[3];

    int ret = g_trans->addTrans(trans); // -1: not logged, 0: no user, 1: success
    if (ret == -1)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 1)
    {
        cerr << "记账成功！（如需查看所有交易内容，请输入acquire)" << endl;
    }
    return ret;
}

int Command::execDelTrans(const vector<string>& args)
{
    // deltrans $no.$
    if (args.size() < 2)
    {
        cerr << "删除交易命令错误，应为：deltrans $no.$" << endl;
        return -1;
    }

    int ret = g_trans->delTrans(atoi(args[1].c_str()) - 1); // -1: not logged, 0: index exceeds range, 1: success
    if (ret == -1)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 0)
    {
        cerr << "交易序号超出范围！" << endl;
    }
    else if (ret == 1)
    {
        cout << "删除交易成功！（如需查看所有交易内容，请输入acquire)" << endl;
    }
    return ret;
}

int Command::execModTrans(const vector<string>& args)
{
    // modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-IN, 1-OUT]
    if (args.size() < 6)
    {
        cerr << "修改交易命令错误，应为：modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-IN, 1-OUT]" << endl;
        return -1;
    }
    UTC utc;
    if (!string2UTC(args[2], utc) || atoi(args[3].c_str()) < 0 || atoi(args[3].c_str()) > 1)
    {
        cerr << "输入日期时间格式错误，应为：$y/m/d-h:m:s$" << endl;
        return 0;
    }

    Transaction trans;
    trans.type = TransType(atoi(args[3].c_str()));
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[4].c_str());
    trans.comment = args[5];
    int ret = g_trans->modTrans(atoi(args[1].c_str() - 1), trans); // -1: not logged, 0: index exceeds range, 1: success
    if (ret == -1)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 0)
    {
        cerr << "交易序号超出范围！" << endl;
    }
    else if (ret == 1)
    {
        cout << "修改交易成功！（如需查看所有交易内容，请输入acquire)" << endl;
    }
    return ret;
}

int Command::execAcquireTrans(const vector<string>& args)
{
    Transactions translist;
    int ret = g_trans->acquire(translist); // -1: not logged, 1: success
    if (ret == -1)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 1)
    {
        if (translist.empty())
        {
            cout << "目前没有交易记录。" << endl;
        }
        else
        {
            printTransactions(translist);
        }
    }
    return ret;
}

void Command::printTransactions(const Transactions& translist)
{
    UTC utc;
    int index = 1;
    cout << "INDEX" << '\t' << "DATE" << '\t' << "TIME" << '\t' << "TYPE" << '\t' << "AMOUNT" << '\t' << "COMMENT" << endl;
    for (auto translog : translist)
    {
        if (!MJD2UTC(translog.mjd, utc))
        {
            index++; // 即使记录错误，序号不要错乱
            continue;
        }
        cout << index++ << '\t' << utc.year << '/' << utc.month << '/' << utc.day << '\t' << utc.hour << ':' << utc.minute << ':' << int(utc.second) << '\t' << ((translog.type == TransType::IN) ? "INCOME" : "EXPENSE") << '\t' << translog.amount << '\t' << translog.comment << endl;
    }
}
