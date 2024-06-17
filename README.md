# 个人财务管理系统项目说明

## 项目简述

**个人财务管理系统**

基于面向对象的设计方法，设计并实现了一个功能完善的个人财务管理系统，旨在帮助用户跟踪和管理其财务状况，实现财务自由。

##### 功能需求

1. **用户账户管理**：
   - 支持用户创建和管理个人账户，并使用密码保护其财务交易信息。
   - 提供登录、登出、修改密码、修改用户名、新增用户、注销用户等功能。
2. **财务交易管理**：
   - 提供用户记录收入、支出功能，支持记录每笔交易的金额、时间、类型、备注等信息。
   - 提供历史交易信息获取、报告生成等功能，方便用户了解自身财务状况。
3. **数据导入/导出**：
   - 支持将财务数据导出为csv文件，方便用户进行数据备份或迁移。
   - 支持从csv文件中导入财务数据，方便用户快速录入历史数据。

##### 框架设计

系统采用分层架构设计，包含数据层、业务层和命令层，涉及四个核心类：

- **DataStorage**：负责账户数据与交易数据的持久管理，提供用户密码校验、用户信息维护、交易数据增删改等接口。
- **User**：负责当前用户信息管理，处理用户登录、注册、信息维护等业务，依赖 DataStorage 类接口实现。
- **Trans**：负责当前用户财务信息管理，处理增加、删除、修改用户交易记录及数据交换等业务，依赖 DataStorage 类接口实现。
- **Command**：负责命令处理，包括命令解析、分配、调用 User 和 Trans 类业务接口等功能。

系统采用命令行界面进行人机交互，提供多种指令，并采用帮助提示与状态反馈的形式帮助用户高效管理个人财务。

<div style="page-break-after:always;"></div>

---

## 运行环境和使用方法

##### 运行环境

- 操作系统：Windows 10，11
- 系统类型：64 位操作系统

- 内存与硬盘：RAM > 100 MB，硬盘可用空间 > 200 MB
- 依赖环境：Microsoft Visual C++ 2015 Redistributable (x64)

##### 使用方法

1. 将可执行程序置于任意文件夹下，在命令提示符（CMD）或 Windows Powershell 中进入文件夹，并执行`.\FinancialManage.exe`即可进入程序主体。

2. 在命令行中输入`help`获取CUI交互命令。命令提示如下：

   ```txt
   用户管理类:
       登录 login $name$ $pswd$
       登出 logout
       修改密码 pswd $oldpswd$ $newpswd$
       修改用户名 name $newname$
       新增用户 newuser $name$ $pswd$
       注销用户 deluser $pswd$
   交易类:
       收入 income $y/m/d-h:m:s$ $amount$ $comment$
       支出 expense $y/m/d-h:m:s$ $amount$ $comment$
       删除交易 deltrans $no.$
       修改交易 modtrans $no.$ $y/m/d-h:m:s$ $type$ $amount$ $comment$
       查看交易 acquire
       查看报告 report
       导出数据 export $filename$
       导入数据 import $filename$
   ```

3. 根据支持的CUI交互命令输入，其中被`$`括起来的部分为用户自定义输入内容。举例如下：

   ```txt
   // 用户类
   newuser user1 password // 新增用户，用户名为user1，密码为password
   login user1 password // 登录，用户名为user1，密码为password
   logout // 登出
   pswd password password1 // 修改密码，需要提供原密码，原密码为password，修改为password1
   deluser password // 注销账户，只能注销当前账户，需要提供密码
   // 交易类 （需要在已登录的前提下）
   income 2024/6/14-13:54:30 100 wage // 记录收入，2024年6月14日13点54分30秒收入100，备注wage
   expense 2024/6/15-20:00:00 50 shopping // 记录支出，2024年6月15日20点支出50，备注shopping
   acquire // 获取当前用户的所有交易记录，列表呈现
   deltrans 1 // 删除序号为1的交易记录
   ```

4. 输入`exit`或`quit`以退出系统。

<div style="page-break-after:always;"></div>

---

## 技术细节

##### 全局变量

系统采用 `extern` 全局变量在程序内部实现数据共享、接口访问等功能。全局变量的实现形式如下（以数据管理类为例）：

```c++
// 声明（DataStorage类头文件中，及其他调用该全局变量的头文件中）
extern DataStorage* g_dataStorage;

// 定义（DataStorage类 .cpp 文件中）
DataStorage* g_dataStorage = new DataStorage();

```

##### DataStorge 类设计

```C++
class DataStorage
{
private:
    vector<UserInfo> user_info;
    map <int, string> uid_map; // uid-->name;
    map <int, Transactions> transactions; // uid-->transaction list

    int readDataFile(string filename = ".\\finacialdata.dat");
    int writeDataFile(string filename = ".\\finacialdata.dat");

    int name2uid(string name, int& uid);
    int uid2name(int uid, string& name);

public:
    DataStorage();
    ~DataStorage();

    int verifyUserPswd(string name, string pswd);
    int getUserUid(string name);

    int addUser(string name, string pswd);
    int delUser(string name, string pswd);
    int modifyName(string oldName, string newName);
    int modifyPswd(string name, string oldPswd, string newPswd);

    int transactionLog(const Transaction& transaction);
    int getUserTransactions(int uid, Transactions& transactions);
    int reorderTransactions(int uid);

    int delTransaction(int uid, int index);
    int modifyTransaction(int uid, int index, const Transaction& newTrans);

    int backupDataFile(string filename);
    int restoreDataFile(string filename);
};
```

DataStorage 类负责账户数据与交易数据的持久管理，内部维护用户与交易信息。

1. 用户信息包括用户名、密码、UID（UID根据时间戳生成，在一台机器上不会重复），采用`vector`存储，并使用`map`管理UID和用户名之间的映射关系。
2. 交易信息包括不同用户的具体交易记录。同一用户的交易记录采用`vector`存储，不同用户的交易记录采用`map`管理。交易记录按照交易时间先后顺序进行排序（`std::sort`）。

DataStorage 类使用文件实现数据持久化管理，并提供用户信息维护、交易信息维护等多个接口。

##### User 类设计

```C++
class User
{
private:
    int uid;
    string name;
    bool logged;
 
public:
    User() : uid(-1), name(""), logged(false) { }
 
    int login(string name, string pswd);
    int logout();
 
    int changePswd(string prePswd, string newPswd);
    int changeName(string newName);
 
    int getUserInfo(int& uid, string& name);
 
    int addUser(string name, string pswd);
    int delUser(string pswd);
};
```

User 类负责当前用户信息管理，处理用户登录、注册、信息维护等业务，依赖 DataStorage 类接口实现，并提供用户信息维护的接口。

##### Trans 类设计

```C++
class Trans
{
private:
    int uid;
    Transactions transactionList;

public:
    Trans() : uid(-1) { };

    int setUid(int uid);
    int refreshTransactionList();
    int addTrans(const Transaction& trans);
    int delTrans(int index);
    int modTrans(int index, const Transaction& newTrans);
    int acquire(Transactions& transList);
};
```

Trans 类负责当前用户财务信息管理，处理增加、删除、修改用户交易记录及数据交换等业务，依赖  DataStorage 类接口实现，并向上提供更加抽象的接口，用于命令具体执行和交易内容的展示。

##### Command 类设计

```c++
class Command
{
private:
    int parseCommand(string command, vector<string>& args);
    COMTYPE com2Type(string com);
    int commandDistribute(const vector<string>& args);
    // User
    int execLogin(const vector<string>& args);
    int execLogout(const vector<string>& args);
    int execChangePswd(const vector<string>& args);
    int execChangeName(const vector<string>& args);
    int execNewUser(const vector<string>& args);
    int execDelUser(const vector<string>& args);
    // Trans
    int execAddTrans(const vector<string>& args, TransType transtype);
    int execDelTrans(const vector<string>& args);
    int execModTrans(const vector<string>& args);
    int execAcquireTrans(const vector<string>& args);
    int execReport(const vector<string>& args);
    int execExport(const vector<string>& args);
    int execImport(const vector<string>& args);
 
    void printTransactions(const Transactions& translist);
    void printReport(const Transactions& translist);
 
    int exportCSV(string filename, const Transactions& translist);
    int importCSV(string filename, Transactions& translist);
 
public:
    int processCommand(string command);
};
```

Command 类负责命令处理，包括命令解析、分配、调用 User 和 Trans 类业务接口等功能，并向上提供用户操作和交易管理的接口。

所有用户目前可以调用的命令如下：

> - 用户管理类:
>
>   - 登录 login \$name\$ \$pswd\$
>   - 登出 logout
>   - 修改密码 pswd \$oldpswd\$ \$newpswd\$
>   - 修改用户名 name \$newname\$
>   - 新增用户 newuser \$name\$ \$pswd\$
>   - 注销用户 deluser \$pswd\$
>
> - 交易类:
>
>   - 收入 income \$y/M/d-h\:m\:s\$ \$amount\$ \$comment\$
>
>   - 支出 expense \$y/M/d-h\:m\:s\$ \$amount\$ \$comment\$
>
>   - 删除交易 deltrans \$no.\$
>
>   - 修改交易 modtrans \$no.\$ \$y/M/d-h\:m\:s\$ \$type\$ \$amount\$ \$comment\$[type:0-IN, 1-OUT]
>
>   - 查看交易 acquire
>
>   - 查看报告 report
>
>   - 导出数据 export \$filename\$
>
>   - 导入数据 import \$filename\$
>
>     *数据文件格式：\$y/M/d-h\:m\:s\$,\$type\$,\$amount\$,\$comment\$*



---

## 不足之处与改进方向

由于项目实施时间较短，项目需求分析与架构设计并不成熟，因此存在一些较明显的不足之出。主要包括下面几点。

- **数据安全**： 目前项目中，用户数据和交易信息采用文本文件明文存储的形式保存，存在数据安全风险。未来，可以采用二进制文件存储、加密算法、CRC校验等方式提高数据安全性，或使用数据库管理系统管理数据。

- **交互形式**： 目前项目采用命令行界面进行交互，用户体验较差。未来，可以考虑使用 MFC 或 Qt 实现图形化界面，提升用户交互体验。
- **功能丰富**： 目前项目功能较为简单，未来可以开发财务状况分析、预算规划、可视化图表、账单支付提醒等功能，不断完善个人财务管理系统的使用体验。
