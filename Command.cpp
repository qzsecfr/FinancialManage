#include "Command.h"
#include <iomanip>
#include <fstream>

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
    if (0 == strcmp(com.c_str(), "newuser")) return COMTYPE::NEWUSER;
    if (0 == strcmp(com.c_str(), "deluser")) return COMTYPE::DELUSER;
    if (0 == strcmp(com.c_str(), "income")) return COMTYPE::INCOME;
    if (0 == strcmp(com.c_str(), "expense")) return COMTYPE::EXPENSE;
    if (0 == strcmp(com.c_str(), "deltrans")) return COMTYPE::DELTRANS;
    if (0 == strcmp(com.c_str(), "modtrans")) return COMTYPE::MODTRANS;
    if (0 == strcmp(com.c_str(), "acquire")) return COMTYPE::ACQUIRE;
    if (0 == strcmp(com.c_str(), "report")) return COMTYPE::REPORT;
    if (0 == strcmp(com.c_str(), "export")) return COMTYPE::EXPORT;
    if (0 == strcmp(com.c_str(), "import")) return COMTYPE::IMPORT;

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
    //      newuser $name$ $pswd$
    //      deluser $pswd$
    //  Trans:
    //      income $y/m/d-h:m:s$ $amount$ $comment$
    //      expense $y/m/d-h:m:s$ $amount$ $comment$
    //      deltrans $no.$
    //      modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-IN, 1-OUT]
    //      acquire
    //      report
    //      export $filename$
    //      import $filename$
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
    case COMTYPE::NEWUSER:
        return execNewUser(args);
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
    case COMTYPE::REPORT:
        return execReport(args);
    case COMTYPE::EXPORT:
        return execExport(args);
    case COMTYPE::IMPORT:
        return execImport(args);
    default:
        break;
    }
    cerr << "无此命令：" << args[0].c_str() << "，请输入help查看帮助。" << endl;
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
    //g_user = getGlobalUser();
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
    if (uid != -1)
    {
        //g_trans = getGlobalTrans();
        g_trans->setUid(uid);
    }
    return ret;
}

int Command::execLogout(const vector<string>& args)
{
    //g_user = getGlobalUser();
    int ret = g_user->logout();
    cout << "已登出！" << endl;
    //g_trans = getGlobalTrans();
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

    //g_user = getGlobalUser();
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

    //g_user = getGlobalUser();
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

int Command::execNewUser(const vector<string>& args)
{
    // newuser $name$ $pswd$
    if (args.size() < 3)
    {
        cerr << "新增用户命令错误，应为：newuser $name$ $pswd$" << endl;
        return -1;
    }

    //g_user = getGlobalUser();
    int ret = g_user->addUser(args[1], args[2]); // -1: illegal name, 0: exist name, 1: success
    if (ret == -1)
    {
        cerr << "用户名中不能出现半角逗号！" << endl;
    }
    else if (ret == 0)
    {
        cerr << "已存在该用户名！" << endl;
    }
    else if (ret == 1)
    {
        int uid = -1;
        string tmp;
        g_user->getUserInfo(uid, tmp);
        if (uid != -1)
        {
            //g_trans = getGlobalTrans();
            g_trans->setUid(uid);
        }
        cout << "新增用户成功，已登录！" << endl;
    }

    return 0;
}

int Command::execDelUser(const vector<string>& args) // -2: not logged, -1: no user, 1: success
{
    //  deluser $pswd$
    if (args.size() < 2)
    {
        cerr << "注销账户命令错误，应为：deluser $pswd$" << endl;
        return -1;
    }

    //g_user = getGlobalUser();
    int ret = g_user->delUser(args[1]);
    if (ret == -2)
    {
        cerr << "尚未登陆！" << endl;
    }
    else if (ret == 1)
    {
        cout << "账户注销成功！" << endl;
        g_trans->setUid(-1);
        
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
    if (atof(args[2].c_str()) < 0)
    {
        cerr << "输入的金额不能为负数！" << endl;
        return 0;
    }

    Transaction trans;
    string tmp;
    g_user->getUserInfo(trans.uid, tmp);
    trans.type = transtype;
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[2].c_str());
    trans.comment = string(args[3]);

    //g_trans = getGlobalTrans();
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

    //g_trans = getGlobalTrans();
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
    if (atof(args[4].c_str()) < 0)
    {
        cerr << "输入的金额不能为负数！" << endl;
        return 0;
    }

    Transaction trans;
    trans.type = TransType(atoi(args[3].c_str()));
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[4].c_str());
    trans.comment = string(args[5]);
    //g_trans = getGlobalTrans();
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
    //g_trans = getGlobalTrans();
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

int Command::execReport(const vector<string>& args)
{
    Transactions translist;
    //g_trans = getGlobalTrans();
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
            printReport(translist);
        }
    }
    return ret;
}

int Command::execExport(const vector<string>& args)
{
    if (args.size() < 2)
    {
        cerr << "导出数据命令错误，应为：export $filename$" << endl;
        return -1;
    }
    Transactions translist;
    //g_trans = getGlobalTrans();
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
            ret = exportCSV(args[1], translist);
            if (!ret)
            {
                cerr << "文件无法打开！" << endl;
            }
            else
            {
                cout << "文件导出成功！" << endl;
            }
        }
    }
    return ret;
}

int Command::execImport(const vector<string>& args)
{
    if (args.size() < 2)
    {
        cerr << "导出数据命令错误，应为：export $filename$" << endl;
        return -1;
    }
    int uid = -1;
    string tmp;
    g_user->getUserInfo(uid, tmp);
    if (uid == -1)
    {
        cerr << "尚未登陆！" << endl;
        return 0;
    }
    else
    {
        Transactions translist;
        int ret = importCSV(args[1], translist);
        if (!ret)
        {
            cerr << "文件无法打开！" << endl;
        }
        else
        {
            for (auto& trans : translist)
            {
                trans.uid = uid;
                ret |= g_trans->addTrans(trans);
            }
            cout << "文件导入成功！" << endl;
        }
        return ret;
    }
}

void Command::printTransactions(const Transactions& translist)
{
    UTC utc;
    int index = 1;
    // width: INDEX:8 DATE:12 TIME:10 TYPE:8 AMOUNT:8
    cout << left << setw(8) << "序号" << setw(12) << "日期" << setw(10) << "时间" << setw(9) << "类型" << setw(8) << "金额"  << "备注" << endl;
    for (auto translog : translist)
    {
        if (!MJD2UTC(translog.mjd, utc))
        {
            index++; // 即使记录错误，序号不要错乱
            continue;
        }
        cout << left << setw(8) << index++
            << setw(4) << utc.year << setw(1) << '/' 
            << right << setfill('0') << setw(2) << utc.month << setw(1) << '/'
            << right << setfill('0') << setw(2) << utc.day << "  "
            << right << setfill('0') << setw(2) << utc.hour << setw(1) << ':'
            << right << setfill('0') << setw(2) << utc.minute << setw(1) << ':'
            << right << setfill('0') << setw(2) << int(utc.second) << "  "
            << left << setfill(' ') << setw(9) << ((translog.type == TransType::IN) ? "INCOME" : "EXPENSE")
            << setw(8) << translog.amount 
            << translog.comment.c_str() << endl;
    }
}

void Command::printReport(const Transactions& translist)
{
    double income = 0, expense = 0;
    for (auto translog : translist)
    {
        income += translog.type ? 0 : translog.amount;
        expense += translog.type ? translog.amount : 0;
    }
    cout << "用户总收入为：" << income << "，总支出为：" << expense << "，总共结余：" << income - expense << endl;
}

int Command::exportCSV(string filename, const Transactions& translist)
{
    // CSV format: (no uid info)
    // $y/M/d-h:m:s$,$type$,$amount$,$comment$
    fstream file;
    file.open(filename, ios::out);
    if (!file.is_open())
    {
        return 0; // file cannot be openned
    }
    UTC utc;
    for (const auto& transaction : translist)
    {
        MJD2UTC(transaction.mjd, utc);
        file << utc.year << '/' << utc.month << '/' << utc.day << '-' << utc.hour << ':' << utc.minute << ':' << utc.second << "," << transaction.type << "," << transaction.amount << "," << transaction.comment.c_str() << std::endl;
    }

    return 1;
}

int Command::importCSV(string filename, Transactions& translist)
{
    fstream file;
    bool fileOpen = false;
    file.open(filename, ios::in);
    fileOpen = file.is_open();
    if (!fileOpen)
    {
        return 0;
    }
    string tmpline;
    Transaction transaction;
    UTC utc;
    while (!file.eof())
    {
        getline(file, tmpline);
        int num = sscanf(tmpline.c_str(), "%d/%d/%d-%d:%d:%lf,%d,%lf,%[^,]", &utc.year, &utc.month, &utc.day, &utc.hour, &utc.minute, &utc.second, &transaction.type, &transaction.amount, transaction.comment.c_str());
        UTC2MJD(utc, transaction.mjd);
        if (num > 8) translist.push_back(transaction);
    }
    return 1;
}
