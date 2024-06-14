#pragma once

#include <iostream>
#include "common.h"

using namespace std;

class DataStorage
{
private:
    vector<UserInfo> user_info;
    map <int, string> uid_map; // uid-->name;
    map <int, Transactions> transactions; // uid-->transaction list

    int readDataFile(string filename = ".\\finacialdata.dat");
    int writeDataFile(string filename = ".\\finacialdata.dat");

    int name2uid(string name, int& uid);
    int uid2name(int uid, string& name);

public:
    DataStorage();
    ~DataStorage();

    int verifyUserPswd(string name, string pswd);
    int getUserUid(string name);

    int addUser(string name, string pswd);
    int delUser(string name, string pswd);
    int modifyName(string oldName, string newName);
    int modifyPswd(string name, string oldPswd, string newPswd);

    int transactionLog(const Transaction& transaction);
    int getUserTransactions(int uid, Transactions& transactions);
    int reorderTransactions(int uid);

    int delTransaction(int uid, int index);
    int modifyTransaction(int uid, int index, const Transaction& newTrans);

    int backupDataFile(string filename);
    int restoreDataFile(string filename);
};

extern DataStorage* g_dataStorage;

//DataStorage* getGlobalDataStorage()
//{
//    if (g_dataStorage == nullptr)
//    {
//        g_dataStorage = new DataStorage();
//    }
//    return g_dataStorage;
//}