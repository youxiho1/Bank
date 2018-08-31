#include "money.h"
#include "CppSQLite3.h"
#include "myTime.h"
#include "rand_string.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//账号、本金、储种、工单号初始化，并存入数据库
Deposit::Deposit(int userAccount, double money, int kind, int staffNumber):userAccount(userAccount),money(money),kind(kind),staffNumber(staffNumber){
    this->depositDate = nowTime_to_str();
    if (kind == 0) {
        this->dueDate = "";
    }else{
        this->dueDate = time_of_months(kind);
    }
    this->flag = 1;
    this->elseMoney = money;
    this->save();
}

Deposit::Deposit(int id){
    CppSQLite3DB db;
    db.open("bank.db");
    
    string sql = "select * from deposit where id = " + to_string(id);
    CppSQLite3Table t = db.getTable(sql.c_str());
    this->Id = id;
    this->userAccount = atoi(t.fieldValue(1));
    this->money = atof(t.fieldValue(2));
    this->depositDate = t.fieldValue(3);
    this->dueDate = t.fieldValue(4);
    this->kind = atoi(t.fieldValue(5));
    this->flag = atoi(t.fieldValue(6));
    this->elseMoney = atof(t.fieldValue(7));
    this->staffNumber = atoi(t.fieldValue(8));
    
}

Deposit::~Deposit(){
}

bool Deposit::save(){
    CppSQLite3DB db;
    db.open("bank.db");
    
    //存入数据库
    string sql = "insert into deposit(useraccount, money, depositdate, duedate, kind, flag, elsemoney, staffnumber)";
    sql += "values(" + to_string(userAccount) + "," + to_string(money) + ",\"" + depositDate + "\",\"" +  dueDate;
    sql += "\"," + to_string(kind) + "," + to_string(flag) + "," + to_string(elseMoney) + "," + to_string(staffNumber) + ")";
    db.execDML(sql.c_str());
    
    //获取此次Deposit的ID
    sql = "select * from deposit where useraccount = " + to_string(userAccount) + " order by id desc";
    CppSQLite3Table t = db.getTable(sql.c_str());
    this->Id = atoi(t.fieldValue(0));
    db.close();
    
    return true;
}

//获取存款信息
int Deposit::get_id(){return this->Id;}

int Deposit::get_userAccount(){return this->userAccount;}

double Deposit::get_money(){return this->money;}

string Deposit::get_depositDate(){return this->depositDate;}

string Deposit::get_dueDate() {return this->dueDate;}

int Deposit::get_kind(){return this->kind;}

int Deposit::get_flag(){return this->flag;}

double Deposit::get_elseMoney(){return this->elseMoney;}

int Deposit::get_staffNumber(){return this->staffNumber;}

DrawingInfo::DrawingInfo(){
}

DrawingInfo::DrawingInfo(int id){       //存款ID，初始化
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from drawinginfo where id = " + to_string(id);
    CppSQLite3Table t = db.getTable(sql.c_str());
    db.close();
    this->depositId = atoi(t.fieldValue(1));
    this->money = atof(t.fieldValue(2));
    this->drawDate = t.fieldValue(3);
    this->elseMoney = atof(t.fieldValue(4)) ;
    this->staffNumber = atoi(t.fieldValue(5));
    this->interest = atof(t.fieldValue(6));
    
}

DrawingInfo::~DrawingInfo(){
}

//获取取款信息
int DrawingInfo::get_depositid(){return this->depositId;}
double DrawingInfo::get_money(){return this->money;}
string DrawingInfo::get_drawDate(){return this->drawDate;}
double DrawingInfo::get_elseMoney(){return this->elseMoney;}
int DrawingInfo::get_staffNumber(){return this->staffNumber;}
double DrawingInfo::get_interest(){return this->interest;}

vector<DrawingInfo> Deposit::get_drawingInfo(){
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from drawinginfo where depositid = " + to_string(this->Id) + " order by id";
    CppSQLite3Table t = db.getTable(sql.c_str());
    for (int i = 0; i < t.numRows(); i++) {
        t.setRow(i);
        this->myDrawingInfo.push_back(DrawingInfo(atoi(t.fieldValue(0))));
    }
    db.close();
    return this->myDrawingInfo;
}

bool DrawingInfo::save(){
    CppSQLite3DB db;
    db.open("bank.db");
    
    //取款表
    string sql = "insert into drawinginfo(depositid,money,drawdate,elsemoney,staffnumber,interest)";
    sql += " values(" + to_string(this->depositId) + "," + to_string(this->money) + ",\"";
    sql += drawDate + "\"," + to_string(this->elseMoney) + "," + to_string(this->staffNumber);
    sql += "," + to_string(interest) + ")";
    db.execDML(sql.c_str());
    
    //存款表更新
    sql = "update deposit set elsemoney = " + to_string(this->elseMoney) +  " where id = " + to_string(this->depositId);
    db.execDML(sql.c_str());
    db.close();
    return true;
}

bool DrawingInfo::draw(int depositId, double draw_money, int staffNumber){            //取款
    this->staffNumber = staffNumber;
    this->drawDate = nowTime_to_str();
    this->depositId = depositId;
    
    CppSQLite3DB db;
    db.open("bank.db");
    string sql = "select * from deposit where id = " + to_string(this->depositId);
    CppSQLite3Table t = db.getTable(sql.c_str());
    
    double last_money = atof(t.fieldValue(7));      //剩余钱数
    int flag = atoi(t.fieldValue(6));               //是否可以取款
    int kind = atoi(t.fieldValue(5));               //储种
    string depositDate = t.fieldValue(3);           //存款时间
    string dueDate = t.fieldValue(4);               //到期时间
    
    if(last_money >= draw_money && flag == 1){
        this->money = draw_money;
        this->elseMoney = last_money - draw_money;
        
        if(kind != 0){  //定期
            string now_date = nowTime_to_str();
            int days = days_of_two_strTime(dueDate, now_date);  //返回从到期时间到现在的天数
            
            //已经到期
            if (days > 0) {
                switch (kind) {
                    case 3:
                        this->interest = draw_money * 3 / 12 * 0.011 + draw_money * 0.0035 * days /365;
                        break;
                    case 6:
                        this->interest = draw_money * 6 / 12 * 0.013 + draw_money * 0.0035 * days /365;
                        break;
                    case 12:
                        this->interest = draw_money * 0.015 + draw_money * 0.0035 * days /365;
                        break;
                    case 24:
                        this->interest = draw_money * 2 * 0.021 + draw_money * 0.0035 * days /365;
                        break;
                    case 36:
                        this->interest = draw_money * 3 * 0.0275 + draw_money * 0.0035 * days /365;
                        break;
                    default:
                        break;
                }
            }else{  //未到期
                string now = nowTime_to_str();
                int last_days = days_of_two_strTime(depositDate, now);
                this->interest = draw_money * 0.0035 * last_days /365;
                
                //设置flag为0
                string sql = "update deposit set flag = 0 where id = " + to_string(this->depositId);
                db.execDML(sql.c_str());
            }
            
            
        }else{      //活期
            string now = nowTime_to_str();
            int days = days_of_two_strTime(depositDate, now);
            this->interest = draw_money * 0.0035 * days / 365;     //利息
        }
        this->save();
        return true;
        
    }
    db.close();
    return false;
}
