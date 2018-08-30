#ifndef STAFF_H
#define STAFF_H

#include <string>

using namespace std;

class Staff
{
public:
    Staff();
    Staff(int);
    ~Staff(){}
    
    bool create_account(int, string, string);   //创建储蓄员
    bool login(int, string);                    //登录
    string make_password(string);               //生成密码
    bool save();                                //存入数据库
    
    //获取储蓄员信息
    int get_number();
    string get_name();
    
private:
    int number;
    string name;
    string hash_password;
    string salt;
    
};
#endif
