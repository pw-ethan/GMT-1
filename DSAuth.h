/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: DSAuth.h
 *  简要描述:.
 *
 *  创建日期: 2016年11月29日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef DSAUTH_H
#define DSAUTH_H

#include <string>
#include <stdint.h>

using namespace std;

class DSAuth{
public:
    DSAuth();
    virtual ~DSAuth();
    void setNum(uint16_t num);
    uint16_t getNum() const;
    void setQueryData(const string & data);
    const string &getQueryData() const;
    void setBrotherData(const string & data);
    const string &getBrotherData() const;
    void setSiblingPath(string * data);
    string * getSiblingPath() const;

private:
    uint16_t num;
    string queryData;
    string brothreData;
    string *siblingPath;
};


#endif

