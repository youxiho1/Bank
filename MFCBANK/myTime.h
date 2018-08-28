#ifndef MYTIME_H
#define MYTIME_H

#include <string>

using namespace std;

time_t str_to_time_t(const string& , const string&);

time_t NowTime();

//把当前日期转换成字符串
string nowTime_to_str();

//计算两个字符串类型的日期之间的天数
int days_of_two_strTime(string , string);

bool is_leap_year(int );

//计算距离当前日期数月后的日期
void get_date(int* , int* , int* );

//返回距离当前日期数月后的日期字符串
string time_of_months(int );

#endif
