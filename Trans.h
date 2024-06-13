#pragma once

#include "DataStorage.h"

class Trans
{
private:
    int uid;
    Transactions transactionList;

public:
    Trans() : uid(-1) { g_dataStorage = getGlobalDataStorage(); };
    ~Trans() { deleteGlobalData(); }
    int setUid(int uid);
    int refreshTransactionList();
    int addTrans(const Transaction& trans);
    int delTrans(int index);
    int modTrans(int index, const Transaction& newTrans);
    int acquire(Transactions& transList);
};

Trans* g_trans = nullptr;

Trans* getGlobalTrans();

void deleteGlobalTrans();