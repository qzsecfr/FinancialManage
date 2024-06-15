#include "DataStorage.h"

#include <fstream>
#include <algorithm>
#include <iomanip>

int DataStorage::readDataFile(string filename)
{
    // 数据文件格式：
    // $USR,uid(int),name(string),pswd(string)
    // $TRA,uid(int),mjd(double),type(int),amount(double),comment(string)
    fstream file;
    bool fileOpen = false;
    file.open(filename, ios::in);
    fileOpen = file.is_open();
    if (!fileOpen)
    {
        if (strcmp(filename.c_str(), ".\\finacialdata.dat") == 0)
        {
            fstream newFile(filename, ios::out);
            newFile.close();
            file.open(filename, ios::in);
            fileOpen = file.is_open();
        }
        if (!fileOpen) return 0; // file cannot be openned
    }

    user_info.clear();
    uid_map.clear();
    transactions.clear();

    string tmpline;
    while (!file.eof())
    {
        getline(file, tmpline);
        if (tmpline.substr(0, 4) == "$USR")
        {
            UserInfo info;
            sscanf(tmpline.c_str(), "$USR,%d,%[^,],%s", &info.uid, info.name.c_str(), info.pswd.c_str());
            user_info.push_back(info);
            if (!uid_map.count(info.uid))
            {
                uid_map[info.uid] = string(info.name);
            }
        }
        else if (strncmp(tmpline.c_str(), "$TRA", 4) == 0)
        {
            Transaction transaction;
            sscanf(tmpline.c_str(), "$TRA,%d,%lf,%d,%lf,%[^,]", &transaction.uid, &transaction.mjd, &transaction.type, &transaction.amount, transaction.comment.c_str());
            if (!transactions.count(transaction.uid))
            {
                transactions[transaction.uid] = Transactions();
            }
            transactions[transaction.uid].push_back(transaction);
        }
    }

    file.close();
    return 1;
}

int DataStorage::writeDataFile(string filename)
{
    fstream file;
    file.open(filename, ios::out);
    if (!file.is_open())
    {
        return 0; // file cannot be openned
    }

    for (const auto& user : user_info)
    {
        file << "$USR," << user.uid << "," << user.name.c_str() << "," << user.pswd.c_str() << std::endl;
    }
    for (const auto& transactionlist : transactions)
    {
        for (const auto& transaction : transactionlist.second)
        {
            file << "$TRA," << transaction.uid << "," << fixed << setprecision(10) << transaction.mjd << "," << transaction.type << "," << transaction.amount << "," << transaction.comment.c_str() << std::endl;
        }
    }

    return 1;
}

int DataStorage::name2uid(string name, int& uid)
{
    for (const auto uidpair : uid_map)
    {
        if (strcmp(name.c_str(), uidpair.second.c_str()) == 0)
        {
            uid = uidpair.first;
            return 1;
        }
    }
    return 0;
}

int DataStorage::uid2name(int uid, string& name)
{
    if (uid_map.count(uid))
    {
        name = string(uid_map[uid]);
        return 1;
    }
    else return 0;
}

DataStorage::DataStorage() : user_info(vector<UserInfo>()), uid_map(map <int, string>()), transactions(map <int, Transactions>())
{
    int ret = readDataFile();
    if (!ret)
    {
        std::cerr << "data storage initial failed." << std::endl;
    }
}

DataStorage::~DataStorage()
{
    int ret = writeDataFile();
    if (!ret)
    {
        std::cerr << "data storage save failed." << std::endl;
    }
}

int DataStorage::verifyUserPswd(string name, string pswd)
{
    int uid = -1;
    if (!name2uid(name, uid))
    {
        return -1; // no user
    }
    for (const auto user : user_info)
    {
        if (user.uid == uid)
        {
            return (strcmp(user.pswd.c_str(), pswd.c_str()) == 0);
        }
    }
    return 0;
}

int DataStorage::getUserUid(string name)
{
    int uid = -1;
    if (!name2uid(name, uid))
    {
        return -1; // no user
    }
    else
    {
        return uid;
    }
}

int DataStorage::addUser(string name, string pswd)
{
    int uid = -1;
    if (name.find(',') != std::string::npos)
    {
        // contain illegal charater ','
        return -1;
    }
    else if (name2uid(name, uid))
    {
        // already exist name
        return 0; 
    }
    uid = uidGenerator();
    UserInfo user;
    user.uid = uid;
    user.name = string(name);
    user.pswd = string(pswd);
    user_info.push_back(user);
    uid_map[uid] = string(name);
    return 1;
}

int DataStorage::delUser(string name, string pswd)
{
    int uid = -1;
    if (!name2uid(name, uid))
    {
        return 0; // no user
    }
    // delete transactions
    transactions.erase(uid);
    // delete user_info
    for (auto iter = user_info.begin(); iter < user_info.end();)
    {
        if (iter->uid == uid)
        {
            iter = user_info.erase(iter);
        }
        else
        {
            iter++;
        }
    }
    uid_map.erase(uid);
    return 1;
}

int DataStorage::modifyName(string oldName, string newName)
{
    int uid = -1;
    if (!name2uid(oldName, uid))
    {
        return -1; // no user
    }
    for (auto &user : user_info)
    {
        if (user.uid == uid)
        {
            user.name = string(newName);
        }
    }
    uid_map[uid] = string(newName);
    return 1;
}

int DataStorage::modifyPswd(string name, string oldPswd, string newPswd)
{
    int uid = -1;
    if (!name2uid(name, uid))
    {
        return -1; // no user
    }
    for (auto &user : user_info)
    {
        if (user.uid == uid)
        {
            if (strcmp(oldPswd.c_str(), user.pswd.c_str()) == 0)
            {
                user.pswd = string(newPswd);
                return 1;
            }
        }
    }
    
    return 0;
}

int DataStorage::transactionLog(const Transaction& log)
{
    if (!uid_map.count(log.uid))
    {
        return 0; // no user;
    }
    if (!transactions.count(log.uid))
    {
        transactions[log.uid] = Transactions();
    }
    transactions[log.uid].push_back(log);
    return 1;
}

int DataStorage::getUserTransactions(int uid, Transactions& transactionlist)
{
    if (!uid_map.count(uid))
    {
        return 0; // no user;
    }
    transactionlist.clear();
    Transaction tmplog;
    for (const auto &translog : transactions[uid])
    {
        tmplog.uid = translog.uid;
        tmplog.type = translog.type;
        tmplog.mjd = translog.mjd;
        tmplog.amount = translog.amount;
        tmplog.comment = string(translog.comment);
        transactionlist.push_back(tmplog);
    }
    return 1;
}

int DataStorage::reorderTransactions(int uid)
{
    // 按照时间先后排序
    if (!uid_map.count(uid))
    {
        return 0; // no user;
    }

    std::sort(transactions[uid].begin(), transactions[uid].end(), compare);
    return 1;
}

int DataStorage::delTransaction(int uid, int index)
{
    if (!uid_map.count(uid) || index < 0 || index >= transactions[uid].size())
    {
        return 0;
    }
    transactions[uid].erase(transactions[uid].begin() + index);
    return 1;
}

int DataStorage::modifyTransaction(int uid, int index, const Transaction& newTrans)
{
    if (!uid_map.count(uid) || index < 0 || index >= transactions[uid].size())
    {
        return 0;
    }
    auto iter = transactions[uid].begin() + index;
    iter->mjd = newTrans.mjd;
    iter->amount = newTrans.amount;
    iter->comment = string(newTrans.comment);
    iter->type = newTrans.type;
    return 1;
}

int DataStorage::backupDataFile(string filename)
{
    int ret = readDataFile(filename);
    if (!ret)
    {
        std::cerr << "data storage initial failed." << std::endl;
    }
    return ret;
}

int DataStorage::restoreDataFile(string filename)
{
    int ret = writeDataFile(filename);
    if (!ret)
    {
        std::cerr << "data storage initial failed." << std::endl;
    }
    return ret;
}

DataStorage* g_dataStorage = new DataStorage();