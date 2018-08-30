#define _CRT_SECURE_NO_WARNINGS
#include "user.h"
#include <string>
#include "CppSQLite3.h"
#include "rand_string.h"
#include "picosha2.h"
#include "myTime.h"

using namespace std;

//初始化user
User::User():Id(0),account(0),name(""),hash_password(""),openDate(""),
			isLost(0),lostDate(""),address(""),idNumber(""),salt("")
			{
}

User::User(int account){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from user where account = " + to_string(account);
    CppSQLite3Table t = db.getTable(sql.c_str());
    this->Id = atoi(t.fieldValue(0));
    this->account = account;
    this->name = t.fieldValue(2);
    this->hash_password = t.fieldValue(3);
    this->openDate = t.fieldValue(4);
    this->isLost = atoi(t.fieldValue(5));
    this->lostDate = t.fieldValue(6);
    this->address = t.fieldValue(7);
    this->idNumber = t.fieldValue(8);
    this->salt = t.fieldValue(9);
    
}

//获取用户数据
int User::get_account(){ return account; }
string User::get_name(){ return name; }
string User::get_openDate(){ return openDate; }
int  User::check_Lost(){ return isLost; }
string User::get_lostDate(){ return lostDate; }
string User::get_address(){ return address; }
string User::get_idNumber(){ return idNumber; }

//获取用户存款信息
vector<Deposit> User::get_Depoist(){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit where useraccount = " + to_string(this->account) + " order by id desc";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        this->myDeposit.push_back(Deposit(atoi(t.fieldValue(0))));
    }
    db.close();
    return  this->myDeposit;
}

//生成密码
string User::make_password(string password){
	salt = rand_str();
	string newPassword = salt + password + salt;
	return picosha2::hash256_hex_string(newPassword.begin(), newPassword.end());
}

//今天开户的用户数量
int num_create_account_of_today(){
    CppSQLite3DB db;
    db.open("bank.db");
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
    CppSQLite3DB db;
    db.open("bank.db");
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
    
    //保证账号唯一
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from user where account = " + to_string(this->account);
    CppSQLite3Table t = db.getTable(sql.c_str());
    while (t.numRows() > 0) {
        sql = "select * from user where account = " + to_string(++this->account);
        t = db.getTable(sql.c_str());
    }
    
    
    save();
    
    sql = "select * from user where account = " + to_string(this->account);
    t = db.getTable(sql.c_str());
    this->Id = atoi(t.fieldValue(0));
    db.close();
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
    CppSQLite3DB db;
    db.open("bank.db");
    string sql ="select * from user where account = " + to_string(account);
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    
    //检查是否挂失
    if (atoi(t.fieldValue(5)) == 1 ) {
        return false;
    }
    
    //检查账号密码
    if(t.numRows() == 1){
        hash_password = t.fieldValue(3);
        salt = t.fieldValue(9);
        string userPassword = salt + password + salt;
        userPassword = picosha2::hash256_hex_string(userPassword.begin(),userPassword.end());
        if (userPassword == hash_password) {
            this->Id = atoi(t.fieldValue(0));
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

void User::change_account(){
    int oldAccount = this->account;
    int newAccoun = newAccount();
    
    //保证账号唯一
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from user where account = " + to_string(newAccoun);
    CppSQLite3Table t = db.getTable(sql.c_str());
    while (t.numRows() > 0) {
        sql = "select * from user where account = " + to_string(++newAccoun);
        t = db.getTable(sql.c_str());
    }
    this->account = newAccoun;
    
    //更新账号,取消挂失
    sql = "update user set account = " + to_string(this->account) + ", islost = " + to_string(0) + " where id = " + to_string(this->Id);
    db.execDML(sql.c_str());
    
    //更新存款信息
    sql  = "select * from deposit where useraccount = " + to_string(oldAccount);
    CppSQLite3Table depositList = db.getTable(sql.c_str());
    vector<int> depositIdList;
    for (int i = 0; i < depositList.numRows(); i++) {//获取存款ID
        depositList.setRow(i);
        depositIdList.push_back(atoi(depositList.fieldValue(0)));
    }
    for (int j = 0; j < depositIdList.size(); j++) {//更新存款账号
        sql = "update deposit set useraccount = " + to_string(this->account) + " where id = " + to_string(depositIdList[j]);
        db.execDML(sql.c_str());
    }
    
    db.close();
}
