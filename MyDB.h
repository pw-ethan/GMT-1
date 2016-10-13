//
// Created by pw-ethan on 16-10-09
//
#pragma once

#include <iostream>
#include <string>
#include <mysql/mysql.h>

using namespace std;

class MyDB
{
    public:
        MyDB();
        virtual ~MyDB();
        bool initDB(string host, string user, string pwd, string db_name);

        
        void startSQL();
        bool insertDB(string tb_name, int index, string content);
        void endSQL(bool ret);
        // bool exeSQL(string sql);
    private:
        MYSQL *connection;
};
