#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

enum TransType
{
    INCOME = 0,
    EXPENSE
};

struct UserInfo
{
    int uid; // 内部使用，由uidgenerater生成
    string name; // 外部使用, 不允许出现半角逗号
    string pswd;
};

struct Transaction
{
    int uid;
    double mjd;
    TransType type;
    double amount;
    string comment;
};
typedef vector<Transaction> Transactions;

class DataStorage
{
private:
    vector<UserInfo> user_info;
    map <int, string> uid_map; // uid-->name;
    map <int, Transactions> transactions; // uid-->transaction list

    int readDataFile(string filename = "./finacialdata.dat");
    int writeDataFile(string filename = "./finacialdata.dat");

    int name2uid(string name, int& uid);
    int uid2name(int uid, string& name);

public:
    DataStorage();
    ~DataStorage();

    int verifyUserPswd(string name, string pswd);

    int addUser(string name, string pswd);
    int delUser(string name, string pswd);

    int transactionLog(const Transaction& transaction);
    int getUserTransactions(int uid, Transactions& transactions);

    int backupDataFile(string filename);
    int restoreDataFile(string filename);
};

