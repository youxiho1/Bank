#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <string>
#include <vector>

using namespace std;

class DrawingInfo
{
public:
    DrawingInfo();
    DrawingInfo(int);               //drawinginfo_ID，初始化,
    bool draw(int,double,int);      //取款
    ~DrawingInfo();
    bool save();                    //存入数据库
    
    //获取取款信息
    int get_depositid();
    double get_money();
    string get_drawDate();
    double get_elseMoney();
    int get_staffNumber();
    double get_interest();
private:
    int depositId;
    double money;
    string drawDate;
    double elseMoney;
    int staffNumber;
    double interest;

};

class Deposit
{
public:
	Deposit(int, double, int, int);     //账号、本金、储种、工单号初始化，并存入数据库
    Deposit(int);                       //使用id初始化，仅用于获取信息
	~Deposit();
    bool save();                        //执行存入数据库操作
    
    //获取该笔存款的取款记录
    vector<DrawingInfo> get_drawingInfo(); 

    //获取存款账单信息
    int get_id();
    int get_userAccount();
    double get_money();
    string get_depositDate();
    string get_dueDate();
    int get_kind();
    int get_flag();
    double get_elseMoney();
    int get_staffNumber();
    
    
private:
    int Id;
	int userAccount;
	double money;
	string depositDate;
	string dueDate;
	int kind;
	int flag;
	double elseMoney;
	int staffNumber;
    vector<DrawingInfo> myDrawingInfo;
	
};

#endif
