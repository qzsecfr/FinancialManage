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
    User() : uid(-1), name(""), logged(false) {}
    ~User() = default;

    int login(string name, string pswd);
    int logout();

    int changePswd(string prePswd, string newPswd);
    int changeName(string newName);

    int getUserInfo(int& uid, string& name);

    int delUser(string pswd);
};

static User* g_user;
