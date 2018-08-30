#ifndef ADMIN_H
#define ADMIN_H

#include <string>
#include <vector>
#include "user.h"
#include "staff.h"

using namespace std;

class Admin{
public:
    Admin();
    ~Admin();
    bool login(string,string);
    vector<User> get_user(string);              //查询用户
    vector<Staff> get_staff();                  //获取全部储蓄员
    double get_sum_of_money();                  //获取银行余额
    double get_today_dueDate_money();           //获取今天到期的金额
    double get_tomorrow_dueDate_money();        //获取明天到期的金额
    double get_after_tomorrow_dueDate_money();  //获取后天到期的金额
    
private:
    string username;
    string password;
    string salt;
    vector<User> myUser;
    vector<Staff> myStaff;
    
};

#endif
