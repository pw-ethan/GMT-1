/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: DBUtility.cpp
 *  简要描述: MySQL数据库工具类
 *
 *  创建日期: 2016年10月9日
 *  作者: pw-ethan
 *  说明: 封装了所需的操作数据库的方法
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/
#include <iostream>
#include <sstream>
#include "DBUtility.h"
#include "config.h"
#include "common.h"

DBUtility::DBUtility(){
    connection = mysql_init(NULL);
    if(connection == NULL){
        //cerr << "[Error] DBUtility::DBUtility() -- mysql_init() is NOT OK!" << endl;
        LOGERROR("[Error] DBUtility::DBUtility() -- mysql_init() is NOT OK!");
   }
}

DBUtility::~DBUtility(){
    if(connection != NULL){
        mysql_close(connection);
    }
    //cout << "[Info] DBUtility::~DBUtility()" << endl;
    LOGINFO("[Info] DBUtility::~DBUtility()");
}

bool DBUtility::initDB(const string & host, const string & user, const string & pwd, const string & db_name){
    connection = mysql_real_connect(connection, host.c_str(),
            user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
    if(connection == NULL){
        cerr << "[Error] DBUtility::initDB() -- mysql_real_connect() is NOT OK!" << endl;
        return false;
    }
    return true;
}
/*
bool DBUtility::exeSQL(string sql){
    if(mysql_query(connection, sql.c_str())){
        cout << "Error: mysql_query() in DBUtility::exeSQL()" << endl;
        return false;
    }else{
        MYSQL_RES *result = mysql_store_result(connection);
        unsigned int num_rows = mysql_num_rows(result);
        unsigned int num_fields = mysql_num_fields(result);
        MYSQL_ROW row;
        for(unsigned int i = 0; i < num_rows; ++i){
            row = mysql_fetch_row(result);
            if(row <= 0){
                break;
            }
            for(unsigned int j = 0; j < num_fields; ++j){
                cout << row[j] << " ";
            }
            cout << endl;
        }
        mysql_free_result(result);
    }
    return true;
}*/

void DBUtility::startSQL(){
    mysql_query(connection, "SET AUTOCOMMIT=0"); // MYSQL will autocommit by default.
    mysql_query(connection, "SET NAMES 'utf8'");
    mysql_query(connection, "START TRANSACTION");
    //cout << "[Info] DBUtility::startSQL() -- start transaction" << endl;
    LOGINFO("[Info] DBUtility::startSQL() -- start transaction");
}

string DBUtility::queryDB(const string & tb_name, const int & index){
    stringstream ss;
    ss << "select weight from " << tb_name <<" where id=" << index;
    string strsql = ss.str();
    //cout << "[Info] " << strsql << endl;
    LOGINFO("[Info] " + strsql);
    if(mysql_real_query(connection, strsql.c_str(), strsql.length())){
        cerr << "[Error] DBUtility::queryDB() -- mysql_real_query() is NOT OK!" << endl;
        return NULL;
    }
    MYSQL_RES * result = mysql_store_result(connection);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row <= 0){
        return NULL;
    }
    ss.str("");
    ss << row[0];
    strsql = ss.str();
    return strsql;
}

bool DBUtility::insertDB(const string & tb_name, const int & index, const string & content){
    stringstream ss;
    ss << "insert into " << tb_name << " values(" << index <<", '" << content << "')";
    string strsql = ss.str();
    //cout << "[Info] " << strsql << endl;
    //LOGINFO("[Info] " + strsql);
    int ret = mysql_real_query(connection, strsql.c_str(), strsql.length());
    if(ret){
        return false;
    }
    return true;
}

bool DBUtility::updateDB(const string & tb_name, const int & index, const string & content){
    stringstream ss;
    ss << "update " << tb_name << " set weight='" << content << "' where id=" << index;
    string strsql = ss.str();
    //cout << "[Info] " << strsql << endl;
    //LOGINFO("[Info] " + strsql);
    int ret = mysql_real_query(connection, strsql.c_str(), strsql.length());
    if(ret){
        return false;
    }
    return true;
}

void DBUtility::endSQL(const bool & ret){
    if(!ret){
        //cout << "[Info] transaction result -- ROLLBACK" << endl;
        LOGINFO("[Info] transaction result -- ROOLBACK");
        mysql_query(connection, "ROLLBACK");
    }else{
        //cout << "[Info] transaction result -- COMMIT" << endl;
        LOGINFO("[Info] transaction result -- COMMIT");
        mysql_query(connection, "COMMIT");
    }
    //cout << "[Info] DBUtility::endSQL() -- end transaction" << endl;
    LOGINFO("[Info] DBUtility::endSQL() -- end transaction");
}

void DBUtility::deleteDB(const string & tb_name){
    stringstream ss;
    ss << "delete from " << tb_name;
    string strsql = ss.str();
    //cout << "[Info] " << strsql << endl;
    LOGINFO("[Info] " + strsql);
    startSQL();
    int ret = mysql_query(connection, strsql.c_str());
    if(ret){
        endSQL(false);
    }
    endSQL(true);
}
