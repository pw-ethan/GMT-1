/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: DSAuth.cpp
 *  简要描述: 
 *
 *  创建日期: 2016年11月29日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include "DSAuth.h"

DSAuth::DSAuth(){}

DSAuth::~DSAuth(){}

void DSAuth::putSomething(std::string k, std::string v){
    pt.put(k, v);
}

std::string DSAuth::getSomething(std::string k){
    std::string _return = pt.get<std::string>(k);
    return _return;
}

std::string DSAuth::toString(){
    std::ostringstream os;
    boost::property_tree::write_json(os, pt, false);
    std::string _return = os.str();
    return _return;
}

void DSAuth::fromString(std::string s){
    std::istringstream is(s);
    boost::property_tree::read_json(is, pt);
}

