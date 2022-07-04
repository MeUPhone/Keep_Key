//#define USER_LOG_IN
//#ifndef USER_LOG_IN

#include "Mysql_OP.h"
#include <iostream>
#include <unistd.h>
using std::cin;
using std::cout;
using std::endl;

Mysql OP;
std::string command;

//注册用户信息
bool Sign_In_User(std::string user_name, std::string user_key) {
    command = "('" + user_name + "', '" + user_key + "')";
    return (OP.InsertData("User_Key", command) && OP.Release());
}

//修改用户登录口令
bool Modify_Key(std::string user_name, std::string user_key) {
    command = "UPDATE User_Key SET user_key = '" + user_key +
              "' where user_name ='" + user_name + "'";
    return (OP.ModifyData(command) && OP.Release());
}

//登录用户
bool Log_In_User(std::string user_name, std::string user_key) {
    //分别检验用户名和登录口令是否一致
    command =
        "SELECT user_name FROM User_Key WHERE user_name = '" + user_name + "'";
    if (!OP.Check_Data(command)) {
        cout << "用户名不存在!请重新输入~" << endl;
        return false;
    } else {
        command = "SELECT user_name FROM User_Key WHERE user_name = '" +
                  user_name + "'" + " AND user_key = '" + user_key + "'";
        if (!OP.Check_Data(command)) {
            cout << "用户名对应登录口令错误!请重新输入~" << endl;
            return false;
        }
    }
    if (!OP.Create_User_Contains_Table(user_name))
        return false;
    cout << "登录成功!请管理您的密码吧~" << endl;
    sleep(2);
    return true;
}

//查看信息
bool Check(std::string user_name) {
    command = "SELECT * FROM " + user_name + "_Contains";
    return (OP.Check_Data(command) && OP.Release());
}

//修改信息
bool Modify_Contains(std::string user_name, int id, std::string name,
                     std::string key, std::string HMAC_name,
                     std::string AES_key, int length) {
    command = "UPDATE " + user_name + "_Contains SET name = '" + name +
              "', _key = '" + key + "', HMAC_name = '" + HMAC_name +
              "', AES_key = '" + AES_key +
              "', key_length = " + std::to_string(length) +
              " where id ='" + std::to_string(id) + "'";
    cout << command << endl;
    return OP.ModifyData(command);

}

//增加信息
bool Add(std::string user_name, int id, std::string name, std::string key,
         std::string HMAC_name, std::string AES_key, int length) {
    std::string UTable = user_name + "_Contains";
    command = "(" + std::to_string(id) + ",'" + name + "','" + key +
              "','" + HMAC_name + "','" + AES_key + "', " +
              std::to_string(length) + ")";
    return OP.InsertData(UTable, command);
}

//删除信息
bool Delete(std::string user_name, int id){
    command = "DELETE FROM " + user_name + "_Contains WHERE id = " + std::to_string(id);
    return OP.ModifyData(command);
}

//#endif