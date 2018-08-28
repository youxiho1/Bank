### 银行流程图

![](https://github.com/youxiho1/Bank/blob/master/bank_flow.png?raw=true)User表

| id       |          | primary key autoincrement | int    |
| -------- | -------- | ------------------------- | ------ |
| account  | 账号     | 至少7位                   | int    |
| name     | 姓名     | 2-10位汉字                | string |
| password | 密码     | 6位数字，如何存储？       | string |
| openDate | 开户日期 |                           | string |
| isLost   | 挂失情况 | 默认0（未挂失）           | int    |
| lostDate | 挂失日期 |                           | string |
| address  | 地址     | 5-50位汉字                | string |
| idNumber | 身份证号 | 18位，最后一位可能是x     | string |
| salt     | 盐       | ？？？？？？？？？？？    | string |

Staff表

| id       |        | primary key autoincrement | int    |
| -------- | ------ | ------------------------- | ------ |
| number   | 工单号 | 8位                       | int    |
| name     | 姓名   | 2-10位汉字                | string |
| password | 密码   | 6-16位，如何存储？？？    | string |

Admin表

| id       |        | primary key autoincrement | int    |
| -------- | ------ | ------------------------- | ------ |
| username | 用户名 |                           | string |
| password | 密码   |                           | string |

Deposit表

| id          |            | primary key autoincrement             | int    |
| ----------- | ---------- | ------------------------------------- | ------ |
| userId      | 谁存的     |                                       | int    |
| money       | 本金       |                                       | real   |
| depositDate | 存款日期   |                                       | string |
| dueDate     | 到期日期   |                                       | string |
| kind        | 储种       | 3 6 12 24 36      0(活期)             | int    |
| flag        | 还能不能取 | 活期永远是1，定期初始是1，取一次变为0 | Int    |
| elseMoney   | 剩余钱数   | （仅做计算余额和明日需要多少钱用）    | real   |
| staffId     | 谁办的     |                                       | int    |

DrawingInfo表

| id        |                                        | primary key autoincrement | int    |
| --------- | -------------------------------------- | ------------------------- | ------ |
| depositId | 存款表中的id号                         |                           | int    |
| money     | 取了多少钱                             |                           | real   |
| drawDate  | 取款时间                               |                           | string |
| elseMoney | 取完之后当时剩下了多少钱（打印存折用） |                           | real   |
| staffId   | 谁办的                                 |                           | int    |

