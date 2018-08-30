#include <string>
#include "admin.h"
#include "CppSQLite3.h"
#include "picosha2.h"
#include "staff.h"
#include "myTime.h"

using namespace std;

Admin::Admin():username(""),password(""),salt(""){
}

Admin::~Admin(){
}

bool Admin::login(string username, string password){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from admin where username = \"" + username + "\"";
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    if (t.numRows() == 1) {
        this->password = t.fieldValue(2);
        this->salt = t.fieldValue(3);
        string userPassword = this->salt + password + this->salt;
        userPassword = picosha2::hash256_hex_string(userPassword.begin(),userPassword.end());
        if (userPassword == this->password) {
            return true;
        }
        return false;
    }
    return false;
}

vector<Staff> Admin::get_staff(){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from staff order by id desc";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        this->myStaff.push_back(Staff(atoi(t.fieldValue(0))));
    }
    return this->myStaff;
}

double Admin::get_sum_of_money(){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit";
    CppSQLite3Table t = db.getTable(sql.c_str());
    double sum_of_money = 0.0;
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        sum_of_money += atof(t.fieldValue(7));
    }
    return sum_of_money;
}

double Admin::get_today_dueDate_money(){
    double sum = 0.0;
    string now = nowTime_to_str();
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit where duedate = \"" + now + "\"";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        sum += atof(t.fieldValue(7));
    }
    db.close();
    return sum;
}

double Admin::get_tomorrow_dueDate_money(){
    double sum = 0.0;
    string now = nowTime_to_str();
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        if (days_of_two_strTime(now, t.fieldValue(4)) == 1) {
            sum += atof(t.fieldValue(7));
        }
    }
    db.close();
    return sum;
}

double Admin::get_after_tomorrow_dueDate_money(){
    double sum = 0.0;
    string now = nowTime_to_str();
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        if (days_of_two_strTime(now, t.fieldValue(4)) == 2) {
            sum += atof(t.fieldValue(7));
        }
    }
    db.close();
    return sum;
}

vector<User> Admin::get_user(string keyword){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from user where account like ";
    sql += "\"%" + keyword + "%\"" + " or name like " + "\"%" + keyword + "%\"";
    sql += " or idnumber like \"%" + keyword + "%\"";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        this->myUser.push_back(User(atoi(t.fieldValue(1))));
    }
    return this->myUser;
}
