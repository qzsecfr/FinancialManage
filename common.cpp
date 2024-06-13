#include "common.h"
#include <ctime>

int uidGenerator()
{
	time_t now = time(nullptr);
	return static_cast<int>(now);
}

int MJD2UTC(double mjd, UTC& utc)
{
	double JD = mjd + 2400000.5;
	double FracDay = mjd - (int)mjd;
	int a = (int)(JD + 0.5);
	int b = a + 1537;
	int c = (int)((b - 122.1) / 365.25);
	int d = (int)(365.25 * c);
	int e = (int)((b - d) / 30.6001);
	int D = (int)(b - d - (int)(30.6001 * e) + (JD + 0.5 - (int)(JD + 0.5)));
	int M = e - 1 - 12 * (int)(e / 14);
	int Y = c - 4715 - (int)((7 + M) / 10);
	utc.year = (unsigned short)Y;
	utc.month = (unsigned short)M;
	utc.day = (unsigned short)D;
	utc.hour = (unsigned short)(FracDay * 24);
	utc.minute = (unsigned short)((FracDay * 24 - utc.hour) * 60);
	utc.second = ((FracDay * 24 - utc.hour) * 60 - utc.minute) * 60.0;
	return 1;
}

int UTC2MJD(UTC utc, double& mjd)
{
	if (utc.month > 2) {
		unsigned short y = utc.year;
		unsigned short m = utc.month;
		unsigned short D = utc.day;
		double UT = utc.hour + utc.minute / 60.0 + utc.second / 3600.0;
		double JD = (int)(365.25 * y) + (int)(30.6001 * (m + 1)) + D + UT / 24.0 + 1720981.5;
		double mjd = JD - 2400000.5;
	}
	else {
		unsigned short y = (unsigned short)(utc.year - 1);
		unsigned short m = (unsigned short)(utc.month + 12);
		unsigned short D = utc.day;
		double UT = utc.hour + utc.minute / 60.0 + utc.second / 3600.0;
		double JD = (int)(365.25 * y) + (int)(30.6001 * (m + 1)) + D + UT / 24.0 + 1720981.5;
		double mjd = JD - 2400000.5;
	}
	return 1;
}

int string2UTC(string str, UTC& utc)
{
	// $y/m/d-h:m:s$
	return sscanf(str.c_str(), "%d/%d/%d-%d:%d:%lf",
		&utc.year, &utc.month, &utc.day, &utc.hour, &utc.minute, &utc.second) == 6;
}

bool compare(const Transaction& a, const Transaction& b)
{
	return a.mjd < b.mjd; // °´ÕÕ mjd ÉýÐòÅÅÁÐ
}
