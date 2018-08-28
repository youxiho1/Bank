#ifndef USER_H
#define USER_H

#include <iostream>
#include <ctime>
#include <string>

using namespace std;

class User
{
public:
  	User();
  	~User(){}

  	//获取用户数据
  	int get_account(){ return account; }
  	string get_name(){ return name; }
  	string get_openDate(){ return openDate; }
  	bool get_isLost(){ return isLost; }
  	string get_lostDate(){ return lostDate; }
  	string get_address(){ return address; }
  	string get_idNumber(){ return idNumber; }

  	//设置用户信息
  	void set_name(string newName){ name = newName; }
  	void set_address(string newAddress){ address = newAddress; }
  	void set_idNumber(string newIdNumber) { idNumber = newIdNumber; }
  	
  	string make_password(string);	//生成密码
  	bool login(int, string);		//判断用户账号与密码，正确则初始化用户变量
  	bool create_account(string,string,string,string);			//创建用户
  	bool save();					//存入数据库
    bool update();                  //更新用户信息
    void set_isLost();              //挂失
    
private:
	int account;
	string name;
	string hash_password;
	string openDate;
	int isLost;
	string lostDate;
	string address;
	string idNumber;
	string salt;
  
};

#endif
