#include "user.h"
#include <iostream>
#include <string>
#include "CppSQLite3.h"
#include "rand_string.h"
#include "picosha2.h"
#include "myTime.h"

using namespace std;

const char* dbFile = "bank.db";     //数据库名
CppSQLite3DB db;                    //数据库类实例

//初始化user
User::User():account(0),name(""),hash_password(""),openDate(""),
			isLost(0),lostDate(""),address(""),idNumber(""),salt("")
			{
}

//生成密码
string User::make_password(string password){
	salt = rand_str();
	string newPassword = salt + password + salt;
	return picosha2::hash256_hex_string(newPassword.begin(), newPassword.end());
}

//今天开户的用户数量
int num_create_account_of_today(){
    db.open(dbFile);
    string sql ="select * from user where opendate = \"" + nowTime_to_str() + "\"" ;
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    return t.numRows();
}

//创建新账号
int newAccount(){
    int number = num_create_account_of_today();
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int year = ltm->tm_year % 100;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    string str_mon;
    if (month < 10) {
        str_mon = "0" + to_string(month);
    }else{
        str_mon = to_string(month);
    }
    string str_day;
    if(day < 10){
        str_day = "0" + to_string(day);
    }else{
        str_day = to_string(day);
    }
    return atoi((to_string(year) + str_mon + str_day + to_string(number)).c_str());
}

//保存到数据库
bool User::save(){
    db.open(dbFile);
    string searchSql = "select * from user where account = " + to_string(account);
    CppSQLite3Table t = db.getTable(searchSql.c_str());
    if (t.numRows() > 0) {
        string sql = "update user set name =  \"" + name + "\",isLost=" + to_string(isLost) + ",";
        sql += "lostDate=\"" + lostDate + "\",address = \"" + address + "\",idnumber = \"";
        sql += idNumber + "\"," + "password = \"" + hash_password + "\"" + ",salt=\"" + salt;
        sql += "\" where account = " + to_string(account);
       
        db.execDML(sql.c_str());
    }
    else{
        string sql = "insert into user(account,name,password,opendate,islost,lostdate,address,idnumber,salt)";
        sql += " values(" + to_string(account) + ",\"" + name + "\",\"" + hash_password + "\",\"" + openDate + "\"," + to_string(isLost);
        sql += ",\"" + lostDate + "\",\"" + address + "\",\"" + idNumber + "\",\"" + salt + "\");";
        db.execDML(sql.c_str());
    }
    db.close();
    return true;
}

//创建用户
bool User::create_account(string newName, string password,string newAddress,string newIdNumber){
    name = newName;
    hash_password = make_password(password);
    address = newAddress;
    idNumber = newIdNumber;
    openDate = nowTime_to_str();
    account = newAccount();
    
    save();
    return true;
}

//设置挂失
void User::set_Lost(){
    isLost = 1;
    lostDate = nowTime_to_str();
    save();
}

//用户登录
bool User::login(int account, string password){
    db.open(dbFile);
    string sql ="select * from user where account = " + to_string(account);
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    if(t.numRows() == 1){
        hash_password = t.fieldValue(3);
        salt = t.fieldValue(9);
        string userPassword = salt + password + salt;
        userPassword = picosha2::hash256_hex_string(userPassword.begin(),userPassword.end());
        if (userPassword == hash_password) {
            this->account = account;
            this->name = t.fieldValue(2);
            this->openDate = t.fieldValue(4);
            this->isLost = atoi(t.fieldValue(5)) ;
            this->lostDate = t.fieldValue(6);
            this->address = t.fieldValue(7);
            this->idNumber = t.fieldValue(8);
            return true;
        }
        return false;
    }else{
        return false;
    }
}

//修改密码
void User::change_password(string newPassword){
    hash_password = make_password(newPassword);
    save();
}

//设置用户信息
void User::set_name(string newName){
    name = newName;
    save();
}
void User::set_address(string newAddress){
    address = newAddress;
    save();
}
void User::set_idNumber(string newIdNumber) {
    idNumber = newIdNumber;
    save();
}
