#pragma once

struct UTC
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    double second;
};

int uidGenerator();

int MJD2UTC(double mjd, UTC& utc);
int UTC2MJD(UTC utc, double& mjd);
