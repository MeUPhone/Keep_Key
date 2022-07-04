//#define USER_INTERFACE_H
//#ifndef USER_INTERFACE_H

#include "Log_IN.h"
#include "SHA-1.h"
#include "AES.h"

int id;
int length;
std::string user_name;
std::string user_key;
std::string name;
std::string key;
std::string HMAC_name;
std::string AES_key;

void Start();
void Sign_In();
void Log_In();
void Manager();

//开始界面:
void Start() {
    //注意清屏函数需要根据环境更改
    system("clear");
    cout << "             欢迎使用密码管理系统!           " << endl;
    cout << "         是否需要注册? 1.是 2.直接登录:      " << endl << endl;
    int i = 0;
    cin >> i;
    i == 1 ? Sign_In() : Log_In();
}

void Sign_In() {
    system("clear");
    cout << "             欢迎使用密码管理系统!           " << endl;
    cout << "                请输入注册信息:             " << endl << endl;
    int i = 0;
    do {
        cout << "用户名:";
        cin >> user_name;
        cout << "登录口令:";
        cin >> user_key;
        SHA sha;
        if (!Sign_In_User(user_name, sha.Use_SHA(user_key))) {
            cout << "该用户名存在注册信息!请更改用户名重新注册或转到登录界面~ "
                    "1.重新注册 2.登录"
                 << endl;
            cin >> i;
        } else
            break;
    } while (i == 1);
    cout << "注册成功!即将前往登录界面~" << endl;
    Log_In();
}

void Log_In() {
    system("clear");
    bool judge;
    cout << "             欢迎使用密码管理系统!           " << endl;
    cout << "                请输入登录信息:             " << endl << endl;
    do {
        cout << "用户名:";
        cin >> user_name;
        cout << "登录口令:";
        cin >> user_key;
        SHA sha;
        judge = Log_In_User(user_name, sha.Use_SHA(user_key));
    } while (!judge);
    Manager();
}

void Manager() {
    system("clear");
    cout << "             欢迎使用密码管理系统!           " << endl;
    cout << "             1.查看密码保存信息             " << endl;
    cout << "             2.修改密码保存信息             " << endl;
    cout << "             3.增添密码保存信息             " << endl;
    cout << "             4.删除密码保存信息             " << endl;
    cout << "             5.修改登录口令                " << endl;
    cout << "             6.登出用户                    " << endl;
    cout << "             7.退出系统                    " << endl << endl;
    int choice = 0;
    do {
        SHA sha;
        cin >> choice;
        switch (choice) {
        case 1:
            Check(user_name);
            break;
        case 2:
            cout<<"请输入所需要修改的用户id:";cin>>id;
            cout<<"修改用户名:";cin>>name;
            cout<<"修改密码:";cin>>key;
            length = key.size();
            HMAC_name = sha.Use_SHA(name);
            //利用拓展后的密码作为明文,登录口令的哈希值作为密钥加密
            AES_key = aes(Limited(key), sha.Use_SHA(user_key));
            Modify_Contains(user_name, id, name, key, HMAC_name, AES_key, length);
            break;
        case 3:
            cout << "请输入所添加的列表信息:" << endl;
            cout << "用户id:";
            cin >> id;
            cout << "用户名:";
            cin >> name;
            cout << "对应密码:";
            cin >> key;
            length = key.size();
            HMAC_name = sha.Use_SHA(name);
            AES_key = aes(Limited(key), sha.Use_SHA(user_key));
            Add(user_name, id, name, key, HMAC_name, AES_key, length);
            break;
        case 4:
            cout <<"请输入所删除的用户id:"<<endl;
            cout <<"用户id:";
            cin>>id;
            Delete(user_name, id);
            break;
        case 5:
            cout << "请输入您的新口令:";
            cin >> user_key;
            if (!Modify_Key(user_name, sha.Use_SHA(user_key)))
                break;
            cout << "修改成功! 请您继续选择:";
        default:
            break;
        }
    } while (choice < 6);
    if (choice == 6)
        Log_In();
    else
        cout << "欢迎下次使用!拜拜~" << endl;
}