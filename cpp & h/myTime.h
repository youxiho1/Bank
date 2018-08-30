#ifndef MYTIME_H
#define MYTIME_H

#include <string>

using namespace std;

time_t str_to_time_t(const string& , const string&);

time_t NowTime();

string nowTime_to_str();

int days_of_two_strTime(string , string);

bool is_leap_year(int );

void get_date(int* , int* , int* );

string time_of_months(int );

#endif
