#include "staff.h"
#include <string>
#include "CppSQLite3.h"
#include "rand_string.h"
#include "picosha2.h"
#include "myTime.h"
#include <iostream>

using namespace std;

Staff::Staff():name(""),hash_password(""),salt(""),number(0){
}

Staff::Staff(int Id){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from staff where id = " + to_string(Id);
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    this->number = atoi(t.fieldValue(1));
    this->name = t.fieldValue(2);
    this->hash_password = t.fieldValue(3);
    this->salt = t.fieldValue(4);
}

//获取信息
int Staff::get_number(){ return number; }
string Staff::get_name(){ return name; }

//生成密码
string Staff::make_password(string password){
    this->salt = rand_str();
    string newPassword = salt + password + salt;
    return picosha2::hash256_hex_string(newPassword.begin(), newPassword.end());
}

bool Staff::save(){
    CppSQLite3DB db;
    db.open("bank.db");
    string searchSql = "select * from staff where number = " + to_string(number);
    CppSQLite3Table t = db.getTable(searchSql.c_str());
    if (t.numRows() > 0) {
        string sql = "update staff set name =  \"" + name + "\",";
        sql += "password = \"" + hash_password + "\"" + ",salt=\"" + salt;
        sql += "\" where number = " + to_string(number);
        db.execDML(sql.c_str());
    }else{
        string sql = "insert into staff(name,password,salt,number)";
        sql += " values(\"" + name + "\",\"" + hash_password + "\",\"" + salt + "\",";
        sql += to_string(number) + ")";
        db.execDML(sql.c_str());
    }
    db.close();
    return true;
}

bool Staff::create_account(int number, string name, string password){
    this->number = number;
    this->name = name;
    this->hash_password = make_password(password);
    save();
    return true;
}

bool Staff::login(int usernumber, string password){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql ="select * from staff where number = " + to_string(usernumber);
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    if(t.numRows() == 1){
        hash_password = t.fieldValue(3);
        salt = t.fieldValue(4);
        string userPassword = salt + password + salt;
        userPassword = picosha2::hash256_hex_string(userPassword.begin(),userPassword.end());
        if (userPassword == hash_password) {
            this->number = usernumber;
            this->name = t.fieldValue(2);
            this->hash_password = t.fieldValue(3);
            this->salt = t.fieldValue(4);
            return true;
        }
        return false;
    }else{
        return false;
    }
}



