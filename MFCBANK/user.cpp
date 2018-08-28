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

User::User():account(0),name(""),hash_password(""),openDate(""),
			isLost(0),lostDate(""),address(""),idNumber(""),salt("")
			{
}

string User::make_password(string password){
	salt = rand_str();
	string newPassword = salt + password + salt;
	hash_password = picosha2::hash256_hex_string(newPassword.begin(), newPassword.end());
	return hash_password;
}

bool User::login(int account, string password){
	db.open(dbFile);
	string sql ="select * from user where account = " + to_string(account);
    CppSQLite3Query q = db.execQuery(sql.c_str());
    for (int i = 0; i < q.numFields(); ++i) {
        if (q.fieldValue(i)!= NULL) {
            cout << q.fieldName(i) << ": "  << q.fieldValue(i) << endl;
        }else{
            cout << q.fieldName(i) << ": "  << "NULL" << endl;
        }
        
    }
    db.close();
    return false;
}

int num_create_account_of_today(){
    db.open(dbFile);
    string sql ="select * from user where opendate = \"" + nowTime_to_str() + "\"" ;
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    return t.numRows();
}

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

bool User::create_account(string newName, string password,string newAddress,string newIdNumber){
    name = newName;
    hash_password = make_password(password);
    address = newAddress;
    idNumber = newIdNumber;
    openDate = nowTime_to_str();
    account = newAccount();
    return true;
}

bool User::save(){
    db.open(dbFile);
    string sql = "insert into user(account,name,password,opendate,islost,lostdate,address,idnumber,salt)";
    sql += " values(" + to_string(account) + ",\"" + name + "\",\"" + hash_password + "\",\"" + openDate + "\"," + to_string(isLost);
    sql += ",\"" + lostDate + "\",\"" + address + "\",\"" + idNumber + "\",\"" + salt + "\");";
    db.execDML(sql.c_str());
    db.close();
    return true;
}
