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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

class DSAuth{
public:
    DSAuth();
    virtual ~DSAuth();
    void putSomething(std::string k, std::string v);
    std::string getSomething(std::string k);
    std::string toString();
    void fromString(std::string s);


private:
    boost::property_tree::ptree pt;
};


#endif

