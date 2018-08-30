#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <ctime>
#include <string>
#include "myTime.h"

using namespace std;

time_t str_to_time_t(const string& ATime, const string& AFormat = "%d-%d-%d")
{
	struct tm tm_Temp;
	time_t time_Ret;
	try
	{
		sscanf(ATime.c_str(), AFormat.c_str(),// "%d/%d/%d %d:%d:%d" ,       
			&(tm_Temp.tm_year),
			&(tm_Temp.tm_mon),
			&(tm_Temp.tm_mday),
			&(tm_Temp.tm_hour),
			&(tm_Temp.tm_min),
			&(tm_Temp.tm_sec),
			&(tm_Temp.tm_wday),
			&(tm_Temp.tm_yday));

		tm_Temp.tm_year -= 1900;
		tm_Temp.tm_mon--;
		tm_Temp.tm_hour = 0;
		tm_Temp.tm_min = 0;
		tm_Temp.tm_sec = 0;
		tm_Temp.tm_isdst = 0;
		time_Ret = mktime(&tm_Temp);
		return time_Ret;
	}
	catch (...) {
		return 0;
	}
}

time_t NowTime()
{
	time_t t_Now = time(0);
	struct tm* tm_Now = localtime(&t_Now);
	tm_Now->tm_hour = 0;
	tm_Now->tm_min = 0;
	tm_Now->tm_sec = 0;
	return  mktime(tm_Now);
}

//把当前日期转换成字符串
string nowTime_to_str() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	string now_str = std::to_string(1900 + ltm->tm_year) + '-';
	now_str += std::to_string(1 + ltm->tm_mon) + '-' + std::to_string(ltm->tm_mday);
	return now_str;
}

//计算两个字符串类型的日期之间的天数
int days_of_two_strTime(string start, string end) {
	time_t a = str_to_time_t(start);
	time_t b = str_to_time_t(end);
	return difftime(b, a) / (24 * 60 * 60);
}

bool is_leap_year(int year) {
	if ((year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0))) {
		return true;
	}
	return false;
}

//计算距离当前日期数月后的日期
void get_date(int* year, int* month, int* day) {
	int days_of_month[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

	*year += *month / 12;
	*month %= 12;

	if (is_leap_year(*year))
		days_of_month[2] = 29;

	if (*day > days_of_month[*month])
		*day = days_of_month[*month];

}

//返回距离当前日期数月后的日期字符串
string time_of_months(int months) {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	int year = 1900 + ltm->tm_year;
	int month = 1 + ltm->tm_mon + months;
	int day = ltm->tm_mday;
	get_date(&year, &month, &day);
	return to_string(year) + "-" + to_string(month) + "-" + to_string(day);
}
