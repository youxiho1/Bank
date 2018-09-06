#include "staff.h"
#include <string>
#include "CppSQLite3.h"
#include "rand_string.h"
#include "picosha2.h"
#include "myTime.h"
#include <iostream>

using namespace std;

Staff::Staff():name(""),hash_password(""),salt(""),number(0),flag(1){
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
    this->flag = atoi(t.fieldValue(5));
}

//获取信息
int Staff::get_number(){ return number; }
string Staff::get_name(){ return name; }
int Staff::get_flag(){ return this->flag; }

//生成密码
string Staff::make_password(string password){
    this->salt = rand_str();
    string newPassword = salt + password + salt;
    return picosha2::hash256_hex_string(newPassword.begin(), newPassword.end());
}

bool Staff::save() {
	CppSQLite3DB db;
	db.open("bank.db");
	string searchSql = "select * from staff where number = " + to_string(number);
	CppSQLite3Table t = db.getTable(searchSql.c_str());
	if (t.numRows() > 0) {
		string sql = "update staff set name =  \"" + name + "\",";
		sql += "password = \"" + hash_password + "\"" + ",salt=\"" + salt + "\", ";
		sql += "flag = " + to_string(this->flag);
		sql += " where number = " + to_string(number);
		db.execDML(sql.c_str());
	}
	else {
		string sql = "insert into staff(name,password,salt,number,flag)";
		sql += " values(\"" + name + "\",\"" + hash_password + "\",\"" + salt + "\",";
		sql += to_string(number) + "," + to_string(this->flag) + ")";
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

bool Staff::login(int usernumber, string password) {
	CppSQLite3DB db;
	db.open("bank.db");
	string sql = "select * from staff where number = " + to_string(usernumber);
	CppSQLite3Table t = db.getTable(sql.c_str());
	db.close();
	if (t.numRows() == 1) {
		//检查是否冻结
		if (atoi(t.fieldValue(5)) == 0) {
			return false;
		}
		hash_password = t.fieldValue(3);
		salt = t.fieldValue(4);
		string userPassword = salt + password + salt;
		userPassword = picosha2::hash256_hex_string(userPassword.begin(), userPassword.end());

		//检查密码是否一致
		if (userPassword == hash_password) {
			this->number = usernumber;
			this->name = t.fieldValue(2);
			this->hash_password = t.fieldValue(3);
			this->salt = t.fieldValue(4);
			return true;
		}
		return false;
	}
	else {
		return false;
	}
}

//冻结账户
bool Staff::set_frozen(){
    this->flag = 0;
    this->save();
    return true;
}

//解冻账户
bool Staff::set_thaw(){
    this->flag = 1;
    this->save();
    return true;
}

//修改密码
bool Staff::change_password(string newPassword){
    this->hash_password = this->make_password(newPassword);
    this->save();
    return true;
}


