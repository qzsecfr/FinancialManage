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
    //      deluser
    //  Trans:
    //      income $y/m/d-h:m:s$ $amount$ $comment$
    //      expense $y/m/d-h:m:s$ $amount$ $comment$
    //      deltrans $no.$
    //      modtrans $no.$ $y/m/f-h:m:s$ $amount$ $comment$
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
