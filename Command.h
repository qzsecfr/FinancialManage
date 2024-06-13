#pragma once

#include "User.h"
#include "Trans.h"

enum COMTYPE
{
    INVALID = -1,

    LOGIN = 0,
    LOGOUT,
    PSWD,
    NAME,
    DELUSER,

    INCOME = 10,
    EXPENSE,
    DELTRANS,
    MODTRANS,
};

class Command
{
private:
    int parseCommand(string command, vector<string>& args);
    COMTYPE com2Type(string com);
    int commandDistribute(const vector<string>& args);
    // User
    int execLogin(const vector<string>& args);
    int execLogout(const vector<string>& args);
    int execChangePswd(const vector<string>& args);
    int execChangeName(const vector<string>& args);
    int execDelUser(const vector<string>& args);
    // Trans
    int execAddTrans(const vector<string>& args, TransType transtype);
    int execDelTrans(const vector<string>& args);
    int execModTrans(const vector<string>& args);

public:
    Command() {}

    int processCommand(string command);
};

static Command* command;