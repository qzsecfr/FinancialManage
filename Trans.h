#pragma once

#include "DataStorage.h"

class Trans
{
private:
    int uid;
    Transactions transactionList;

public:
    Trans() : uid(-1) { /*g_dataStorage = getGlobalDataStorage();*/ };

    int setUid(int uid);
    int refreshTransactionList();
    int addTrans(const Transaction& trans);
    int delTrans(int index);
    int modTrans(int index, const Transaction& newTrans);
    int acquire(Transactions& transList);
};

extern Trans* g_trans;
extern DataStorage* g_dataStorage;

//Trans* getGlobalTrans()
//{
//    if (g_trans == nullptr)
//    {
//        g_trans = new Trans;
//    }
//    return g_trans;
//}
