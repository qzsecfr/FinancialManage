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
    NEWUSER,
    DELUSER,

    INCOME = 10,
    EXPENSE,
    DELTRANS,
    MODTRANS,
    ACQUIRE,
    REPORT,
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
    int execNewUser(const vector<string>& args);
    int execDelUser(const vector<string>& args);
    // Trans
    int execAddTrans(const vector<string>& args, TransType transtype);
    int execDelTrans(const vector<string>& args);
    int execModTrans(const vector<string>& args);
    int execAcquireTrans(const vector<string>& args);
    int execReport(const vector<string>& args);

    void printTransactions(const Transactions& translist);
    void printReport(const Transactions& translist);

public:
    int processCommand(string command);
};
