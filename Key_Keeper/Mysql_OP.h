//#define MYSQL_OPERATE_H
//#ifndef MYSQL_OPERATE_H

#include <iostream>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

class Mysql {
  public:
    Mysql() {
        ConnectDatabase();
        Create_User_Key_Table();
    }

    ~Mysql() { mysql_close(&mysql); }

    //连接数据库
    bool ConnectDatabase() {
        //初始化mysql
        mysql_init(&mysql); //连接mysql，数据库为Keep_Key
        const char host[] = "localhost";
        const char user[] = "root";
        const char psw[] = "mi020418";
        const char database[] = "Keep_Key";
        const int port = 0;
        //返回false则连接失败，返回true则连接成功
        if (!mysql_real_connect(&mysql, host, user, psw, database, port, NULL,
                                CLIENT_FOUND_ROWS))
        //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
        {
            printf("Error connecting to database:%s\n", mysql_error(&mysql));
            return false;
        } else {
            printf("Connected...\n");
            return true;
        }
    }

    bool Release() {
        mysql_free_result(res);
        return true;
    }

    // //通用接口
    // bool Query(std::string command) {
    //     strcpy(query, command.data());
    //     std::cout << query << std::endl;
    //     if (mysql_query(&mysql, query)) {
    //         printf("Query failed (%s)\n", mysql_error(&mysql));
    //         return false;
    //     } else {
    //         printf("query success\n");
    //     }
    //     //获取结果集
    //     if (!(res = mysql_store_result(&mysql)))
    //     //获得sql语句结束后返回的结果集
    //     {
    //         printf("Couldn't get result from %s\n", mysql_error(&mysql));
    //         Release();
    //         return false;
    //     }

    //     //打印数据行数
    //     printf("number of dataline returned: %ld\n",
    //            mysql_affected_rows(&mysql));
    //     if (mysql_affected_rows(&mysql) == 0)
    //         return false;
    //     return true;
    // }

    //创建登录者和口令的Table
    void Create_User_Key_Table() {
        std::string command = "CREATE TABLE IF NOT EXISTS `User_Key`(user_name "
                              "char(50) NOT NULL, user_key char(50) NOT NULL "
                              "DEFAULT 123456, PRIMARY KEY(user_name)) ";
        strcpy(query, command.data());
        mysql_query(&mysql, query);
        return;
    }

    //创建每个登录者包含内容的Table
    bool Create_User_Contains_Table(std::string user_name) {
        std::string head = "CREATE TABLE IF NOT EXISTS `";
        std::string back = "_Contains`(id int NOT NULL,name char(50) NOT "
                           "NULL,_key char(32) NOT NULL,HMAC_name char(50) "
                           "NULL,AES_key char(32) NULL,key_length int "
                           "NULL,PRIMARY KEY(id))";
        std::string command = head + user_name + back;
        strcpy(query, command.data());
        if (mysql_query(&mysql, query)) //执行SQL语句
        {
            printf("Query failed (%s)\n", mysql_error(&mysql));
            return false;
        } else {
            printf("Insert success\n");
            return true;
        }
    }

    //插入数据
    bool InsertData(std::string table, std::string value) {
        std::string command = "INSERT INTO " + table + " VALUES" + value;
        strcpy(query, command.data());

        if (mysql_query(&mysql, query)) //执行SQL语句
        {
            printf("Query failed (%s)\n", mysql_error(&mysql));
            return false;
        } else {
            printf("Insert success\n");
            return true;
        }
    }

    //修改数据
    bool ModifyData(std::string command) {
        strcpy(query, command.data());
        if (mysql_query(&mysql, query)) //执行SQL语句
        {
            printf("Query failed (%s)\n", mysql_error(&mysql));
            return false;
        } else {
            printf("Modifing success\n");
            return true;
        }
    }

    //打印信息
    bool Check_Data(std::string command) {
        strcpy(query, command.data());
        //返回0 查询成功，返回1查询失败
        if (mysql_query(&mysql, query)) //执行SQL语句
        {
            printf("Query failed (%s)\n", mysql_error(&mysql));
            printf("当前用户未创建密码信息!\n");
            return false;
        } else {
            printf("query success\n");
        }
        res = mysql_store_result(&mysql);
        //打印数据行数
        printf("number of dataline returned: %ld\n",
               mysql_affected_rows(&mysql));
        if (mysql_affected_rows(&mysql) == 0) {
            std::cout << "无信息!" << std::endl;
            return false;
        }

        for (int i = 0; fd = mysql_fetch_field(res); i++) //获取字段名
            strcpy(field[i], fd->name);
        int j = mysql_num_fields(res); // 获取列数
        for (int i = 0; i < j; i++) {  //打印字段
            if (i == 2 || i == 4)
                printf("%10s\t\t\t", field[i]);
            else if(i == 3)printf("%10s\t\t\t\t   ", field[i]);
            else printf("%10s\t", field[i]);
        }
        printf("\n");
        while (column = mysql_fetch_row(res)) {
            for (int i = 0; i < j; i++) {
                if (i == 5)
                    printf("%10s", column[i]);
                else printf("%10s\t", column[i]);
            }
            printf("\n");
        }
        return true;
    }

  private:
    MYSQL mysql;        //创建一个连接,在主函数最后释放
    char query[500];    //请求语句
    MYSQL_FIELD *fd;    //字段列数组
    char field[32][32]; //存字段名二维数组
    MYSQL_RES *res;     //这个结构代表返回行的一个查询结果集
    MYSQL_ROW column; //一个行数据的类型安全(type-safe)的表示，表示数据行的列
};

// /***************************数据库操作***********************************/
// //其实所有的数据库操作都是先写个sql语句，然后用mysql_query(&mysql,query)来完成，包括创建数据库或表，增删改查
// //查询数据
// bool QueryDatabase1(std::string query_command) {
//     strcpy(query, query_command.data());
//     //执行查询语句，这里是查询所有，user是表名，不用加引号，用strcpy也可以
//     mysql_query(
//         &mysql,
//         "set names gbk"); //设置编码格式（SET NAMES
//         GBK也行），否则cmd下中文乱码    //插入行

//     执行指定为一个空结尾的字符串的SQL查询。
//         // printf("Query failed (%s)\n", mysql_error(&mysql));
//         return false;
//     } /*else {
//         printf("query success\n");
//     }*/

//     //获取结果集
//     if (!(res = mysql_store_result(&mysql))) //获得sql语句结束后返回的结果集
//     {
//         //printf("Couldn't get result from %s\n", mysql_error(&mysql));
//         return false;
//     }

//     //打印数据行数
//     printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

//     //获取字段的信息
//     char *str_field[32];        //定义一个字符串数组存储字段信息
//     for (int i = 0; i < 4; i++) //在已知字段数量的情况下获取字段名
//     {
//         str_field[i] =
//             mysql_fetch_field(res)->name; //返回一个所有字段结构的数组。
//     }
//     for (int i = 0; i < 4; i++) //打印字段
//         printf("%10s\t", str_field[i]);
//     printf("\n");
//     //打印获取的数据
//     while (column =
//                mysql_fetch_row(res)) //在已知字段数量情况下，获取并打印下一行
//     {
//         printf("%10s\t%10s\t%10s\t%10s\n", column[0], column[1], column[2],
//                column[3]); // column是列数组
//     }
//     return true;
// }

// bool QueryDatabase2() {
//     mysql_query(&mysql, "set names gbk");
//     //返回0 查询成功，返回1查询失败
//     if (mysql_query(&mysql, "select * from user")) //执行SQL语句
//     {
//         printf("Query failed (%s)\n", mysql_error(&mysql));
//         return false;
//     } else {
//         printf("query success\n");
//     }
//     res = mysql_store_result(&mysql);
//     //打印数据行数
//     printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));
//     for (int i = 0; fd = mysql_fetch_field(res); i++) //获取字段名
//         strcpy(field[i], fd->name);
//     int j = mysql_num_fields(res); // 获取列数
//     for (int i = 0; i < j; i++)    //打印字段
//         printf("%10s\t", field[i]);
//     printf("\n");
//     while (column = mysql_fetch_row(res)) {
//         for (int i = 0; i < j; i++)
//             printf("%10s\t", column[i]);
//         printf("\n");
//     }
//     return true;
// }

// //删除数据
// bool DeleteData() {
//     /*sprintf(query, "delete from user where id=6");*/
//     char query[100];
//     printf("please input the sql:\n");
//     //getline(query);                    //这里手动输入sql语句
//     if (mysql_query(&mysql, query)) //执行SQL语句
//     {
//         printf("Query failed (%s)\n", mysql_error(&mysql));
//         return false;
//     } else {
//         printf("Insert success\n");
//         return true;
//     }
// }

//#endif