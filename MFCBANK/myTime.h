#ifndef MYTIME_H
#define MYTIME_H

#include <string>

using namespace std;

time_t str_to_time_t(const string& , const string&);

time_t NowTime();

//�ѵ�ǰ����ת�����ַ���
string nowTime_to_str();

//���������ַ������͵�����֮�������
int days_of_two_strTime(string , string);

bool is_leap_year(int );

//������뵱ǰ�������º������
void get_date(int* , int* , int* );

//���ؾ��뵱ǰ�������º�������ַ���
string time_of_months(int );

#endif
