#ifndef USER_H
#define USER_H

#include <iostream>
#include <ctime>
#include <string>
#include "money.h"
#include <vector>

using namespace std;

class User
{
public:
  	User();         //初始化，变量设为null
    User(int);      //使用账号初始化，变量从数据库提取赋值
    
  	~User(){}

  	//获取用户数据
    int get_account();
    string get_name();
    string get_openDate();
    int  check_Lost();
    string get_lostDate();
    string get_address();
    string get_idNumber();
    
    //获取存款信息
    vector<Deposit> get_Depoist();

  	//设置用户信息
    void set_name(string newName);
    void set_address(string newAddress);
    void set_idNumber(string newIdNumber);
  	
  	string make_password(string);	                    //生成密码
  	bool login(int, string);		                    //判断用户账号与密码，正确则初始化用户变量
  	bool create_account(string,string,string,string);	//创建用户
  	bool save();					                    //存入数据库
    void set_Lost();                                    //挂失
    void change_password(string);                       //修改密码
    void change_account();                              //修改账号，以及账号对应的存款
    
private:
    int Id;
	int account;
	string name;
	string hash_password;
	string openDate;
	int isLost;
	string lostDate;
	string address;
	string idNumber;
	string salt;
    vector<Deposit> myDeposit;
  
};
#endif
