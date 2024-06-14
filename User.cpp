#include "User.h"

int User::login(string name, string pswd)
{
    //g_dataStorage = getGlobalDataStorage();
    int ret = g_dataStorage->verifyUserPswd(name, pswd); // -1:no user, 0:fail, 1:success
    if (ret)
    {
        this->name = string(name);
        this->uid = g_dataStorage->getUserUid(name);
        this->logged = true;
    }
    return ret;
}

int User::logout()
{
    this->name = "";
    this->uid = -1;
    this->logged = false;
    return 1;
}

int User::changePswd(string prePswd, string newPswd) // -2: not logged, -1: no user, 0: wrong pswd, 1: success
{
    if (!logged)
    {
        return -2;
    }
    else
    {
        //g_dataStorage = getGlobalDataStorage();
        return g_dataStorage->modifyPswd(name, prePswd, newPswd);
    }
}

int User::changeName(string newName) // -2: not logged, -1: no user, 1: success
{
    if (!logged)
    {
        return -2;
    }
    else
    {
        //g_dataStorage = getGlobalDataStorage();
        int ret = g_dataStorage->modifyName(name, newName);
        if (ret == 1)
        {
            name = string(newName);
        }
        return ret;
    }
}

int User::getUserInfo(int& uid, string& name)
{
    if (!logged)
    {
        return -2;
    }
    else
    {
        uid = this->uid;
        name = string(this->name);
        return 1;
    }
}

int User::delUser(string pswd) // -2: not logged, -1: no user, 1: success
{
    if (!logged)
    {
        return -2;
    }
    else
    {
        //g_dataStorage = getGlobalDataStorage();
        int ret = g_dataStorage->delUser(name, pswd);
        logout();
        return ret;
    }
}

int User::addUser(string name, string pswd)
{
    if (logged)
    {
        logout();
    }
    //g_dataStorage = getGlobalDataStorage();
    int ret = g_dataStorage->addUser(name, pswd); // -1: illegal name, 0: exist name, 1: success
    if (ret == 1)
    {
        login(name, pswd);
    }
    return ret;
}

User* g_user = new User();