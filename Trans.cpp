#include "Trans.h"

int Trans::setUid(int uid)
{
    this->uid = uid;
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
        return g_dataStorage->getUserTransactions(uid, transactionList);
    }
}

int Trans::addTrans(const Transaction& trans)
{
    if (uid == -1)
    {
        return -1;
    }
    int ret = g_dataStorage->transactionLog(trans);
    refreshTransactionList();
    return ret;
}

int Trans::delTrans(int index)
{
    if (uid == -1 || index < 0 || index >= transactionList.size())
    {
        return -1;
    }
    int ret = g_dataStorage->delTransaction(uid, index);
    refreshTransactionList();
    return ret;
}

int Trans::modTrans(int index, const Transaction& newTrans)
{
    if (uid == -1 || index < 0 || index >= transactionList.size())
    {
        return -1;
    }
    int ret = g_dataStorage->modifyTransaction(uid, index, newTrans);
    refreshTransactionList();
    return ret;
}
