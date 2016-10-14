/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: DBUtility.h
 *  简要描述: MySQL数据库工具类
 *
 *  创建日期: 2016年10月9日
 *  作者: pw-ethan
 *  说明: 封装了所需的操作数据库的方法
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/

#pragma once

#include <iostream>
#include <string>
#include <mysql/mysql.h>

using namespace std;

class DBUtility
{
    public:
        DBUtility();
        virtual ~DBUtility();
        bool initDB(const string & host, const string & user, const string & pwd, const string & db_name);

        
        void startSQL();
        string queryDB(const string & tb_name, const int & index);
        bool insertDB(const string & tb_name, const int & index, const string & content);
        bool updateDB(const string & tb_name, const int & index, const string & content);
        void endSQL(const bool & ret);
        // bool exeSQL(string sql);
    private:
        MYSQL *connection;
};
