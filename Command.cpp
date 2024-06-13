#include "Command.h"

int Command::processCommand(string command)
{
    vector<string> args;
    if (!parseCommand(command, args))
    {
        return -1; // command error
    }

    return commandDistribute(args);
}

int Command::parseCommand(string command, vector<string>& args)
{
    size_t start = 0;
    size_t end = 0;
    args.clear();
    while (end != std::string::npos)
    {
        end = command.find_first_of(" \t\n", start);
        if (end == std::string::npos)
        {
            end = command.length();
        }

        args.push_back(command.substr(start, end - start));

        start = end + 1;
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
    //      modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-INCOME, 1-EXPENSE]
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
        return execAddTrans(args, TransType::INCOME);
    case COMTYPE::EXPENSE:
        return execAddTrans(args, TransType::EXPENSE);
    case COMTYPE::DELTRANS:
        return execDelTrans(args);
    case COMTYPE::MODTRANS:
        return execModTrans(args);
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
        return -1;
    }
    
    return g_user->login(args[1], args[2]);
}

int Command::execLogout(const vector<string>& args)
{
    return g_user->logout();
}

int Command::execChangePswd(const vector<string>& args)
{
    // pswd $oldpswd$ $newpswd$
    if (args.size() < 3)
    {
        return -1;
    }

    return g_user->changePswd(args[1], args[2]);
}

int Command::execChangeName(const vector<string>& args)
{
    // name $newname$
    if (args.size() < 2)
    {
        return -1;
    }

    return g_user->changeName(args[1]);
}

int Command::execDelUser(const vector<string>& args)
{
    //  deluser $pswd$
    if (args.size() < 2)
    {
        return -1;
    }

    return g_user->delUser(args[1]);
}

int Command::execAddTrans(const vector<string>& args, TransType transtype)
{
    // income $y/m/d-h:m:s$ $amount$ $comment$
    // expense $y/m/d-h:m:s$ $amount$ $comment$
    if (args.size() < 4)
    {
        return -1;
    }
    UTC utc;
    if (!string2UTC(args[1], utc))
    {
        return 0;
    }

    Transaction trans;
    trans.type = transtype;
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[2].c_str());
    trans.comment = args[3];

    return g_trans->addTrans(trans);
}

int Command::execDelTrans(const vector<string>& args)
{
    // deltrans $no.$
    if (args.size() < 2)
    {
        return -1;
    }

    return g_trans->delTrans(atoi(args[1].c_str()));
}

int Command::execModTrans(const vector<string>& args)
{
    // modtrans $no.$ $y/m/f-h:m:s$ $type$ $amount$ $comment$ [type:0-INCOME, 1-EXPENSE]
    if (args.size() < 6)
    {
        return -1;
    }
    UTC utc;
    if (!string2UTC(args[2], utc) || atoi(args[3].c_str()) < 0 || atoi(args[3].c_str()) > 1)
    {
        return 0;
    }

    Transaction trans;
    trans.type = TransType(atoi(args[3].c_str()));
    UTC2MJD(utc, trans.mjd);
    trans.amount = atof(args[4].c_str());
    trans.comment = args[5];
    return g_trans->modTrans(atoi(args[1].c_str()), trans);
}

Command* comman = new Command();