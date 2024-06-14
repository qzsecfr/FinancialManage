#include "Trans.h"

int Trans::setUid(int uid)
{
    this->uid = uid;
    refreshTransactionList();
    return 1;
}

int Trans::refreshTransactionList()
{
    if (uid == -1)
    {
        transactionList.clear();
        return 1;
    }
    else
    {
        //g_dataStorage = getGlobalDataStorage();
        return g_dataStorage->getUserTransactions(uid, transactionList);
    }
}

int Trans::addTrans(const Transaction& trans) // -1: not logged, 0: no user, 1: success
{
    if (uid == -1)
    {
        return -1;
    }
    //g_dataStorage = getGlobalDataStorage();
    int ret = g_dataStorage->transactionLog(trans);
    g_dataStorage->reorderTransactions(uid);
    refreshTransactionList();
    return ret;
}

int Trans::delTrans(int index) // -1: not logged, 0: index exceeds range, 1: success
{
    if (uid == -1)
    {
        return -1;
    }
    else if (index < 0 || index >= transactionList.size())
    {
        return 0;
    }
    //g_dataStorage = getGlobalDataStorage();
    int ret = g_dataStorage->delTransaction(uid, index);
    g_dataStorage->reorderTransactions(uid);
    refreshTransactionList();
    return ret;
}

int Trans::modTrans(int index, const Transaction& newTrans) // -1: not logged, 0: index exceeds range, 1: success
{
    if (uid == -1)
    {
        return -1;
    }
    else if (index < 0 || index >= transactionList.size())
    {
        return 0;
    }
    //g_dataStorage = getGlobalDataStorage();
    int ret = g_dataStorage->modifyTransaction(uid, index, newTrans);
    g_dataStorage->reorderTransactions(uid);
    refreshTransactionList();
    return ret;
}

int Trans::acquire(Transactions& transList) // -1: not logged, 1: success
{
    if (uid == -1)
    {
        return -1;
    }
    transList = transactionList;
    return 1;
}

Trans* g_trans = new Trans();