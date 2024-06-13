#include "User.h"

int User::login(string name, string pswd)
{
    int ret = g_dataStorage->verifyUserPswd(name, pswd); // -1:no user, 0:fail, 1:success
    if (ret)
    {
        this->name = name;
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

int User::changePswd(string prePswd, string newPswd)
{
    if (!logged)
    {
        return 0;
    }
    else
    {
        return g_dataStorage->modifyPswd(name, prePswd, newPswd);
    }
}

int User::changeName(string newName)
{
    if (!logged)
    {
        return 0;
    }
    else
    {
        return g_dataStorage->modifyName(name, newName);
    }
}

int User::getUserInfo(int& uid, string& name)
{
    if (!logged)
    {
        return 0;
    }
    else
    {
        uid = this->uid;
        name = string(this->name);
        return 1;
    }
}

int User::delUser(string pswd)
{
    if (!logged)
    {
        return 0;
    }
    else
    {
        return g_dataStorage->delUser(name, pswd);
    }
}
