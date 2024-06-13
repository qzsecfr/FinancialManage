#pragma once

#include "DataStorage.h"

class Trans
{
private:
    int uid;
    Transactions transactionList;

public:
    Trans() : uid(-1) {};
    ~Trans() = default;

    int setUid(int uid);
    int refreshTransactionList();
    int addTrans(const Transaction& trans);
    int delTrans(int index);
    int modTrans(int index, const Transaction& newTrans);

};

static Trans* g_trans = new Trans();