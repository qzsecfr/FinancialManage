#pragma once
#pragma warning(disable:4996)

#include <string>
#include <vector>
#include <map>

#define _CRT_SECURE_NO_WARNINGS

using namespace std;

struct UTC
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
};

enum TransType
{
    IN = 0,
    OUT
};

struct UserInfo
{
    int uid; // �ڲ�ʹ�ã���uidgenerater����
    string name; // �ⲿʹ��, ��������ְ�Ƕ���
    string pswd;
};

struct Transaction
{
    int uid;
    double mjd;
    TransType type;
    double amount;
    string comment;
};
typedef vector<Transaction> Transactions;

int uidGenerator();

int MJD2UTC(double mjd, UTC& utc);
int UTC2MJD(UTC utc, double& mjd);
int string2UTC(string str, UTC& utc);

bool compare(const Transaction& a, const Transaction& b);
