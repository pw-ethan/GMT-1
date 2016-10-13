#include <iostream>
#include <sstream>
#include "MyDB.h"
#include "config.h"

MyDB::MyDB(){
    connection = mysql_init(NULL);
    if(connection == NULL){
        cerr << "Error: mysql_init() in MyDB::MyDB()" << endl;
    }
}

MyDB::~MyDB(){
    if(connection != NULL){
        mysql_close(connection);
    }
    cout << "MyDB::~MyDB()" << endl;
}

bool MyDB::initDB(string host, string user, string pwd, string db_name){
    connection = mysql_real_connect(connection, host.c_str(),
            user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);
    if(connection == NULL){
        cerr << "Error: mysql_real_connect() in MyDB::initDB()" << endl;
        return false;
    }
    return true;
}
/*
bool MyDB::exeSQL(string sql){
    if(mysql_query(connection, sql.c_str())){
        cout << "Error: mysql_query() in MyDB::exeSQL()" << endl;
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

void MyDB::startSQL(){
    mysql_query(connection, "SET AUTOCOMMIT=0"); // MYSQL will autocommit by default.
    mysql_query(connection, "START TRANSACTION");
    cout << "start transaction" << endl;
}

bool MyDB::insertDB(string tb_name, int index, string content){
    stringstream ss;
    ss << "insert into " << tb_name << " values(" << index <<", '" << content << "')";
    string strsql = ss.str();
    cout << strsql << endl;
    int ret = mysql_query(connection, strsql.c_str());
    if(ret){
        return false;
    }
    return true;
}

void MyDB::endSQL(bool ret){
    if(!ret){
        mysql_query(connection, "ROLLBACK");
    }else{
        mysql_query(connection, "COMMIT");
    }
    cout << "end transaction" << endl;
}
