#pragma once
#include "DataStorage.h"

using namespace std;

class User
{
private:
    int uid;
    string name;
    bool logged;

public:
    User() : uid(-1), name(""), logged(false) { /*g_dataStorage = getGlobalDataStorage();*/ }

    int login(string name, string pswd);
    int logout();

    int changePswd(string prePswd, string newPswd);
    int changeName(string newName);

    int getUserInfo(int& uid, string& name);

    int addUser(string name, string pswd);
    int delUser(string pswd);
};

extern User* g_user;
extern DataStorage* g_dataStorage;

//User* getGlobalUser()
//{
//    if (g_user == nullptr)
//    {
//        g_user = new User();
//    }
//    return g_user;
//}
