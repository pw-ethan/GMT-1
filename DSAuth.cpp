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

DSAuth::DSAuth(): num(0), siblingPath(0){

}

DSAuth::~DSAuth(){}

void DSAuth::setNum(uint16_t num){
    this->num = num;
}

uint16_t DSAuth::getNum() const {
    return this->num;
}

void DSAuth::setQueryData(const string & data){
    this->queryData = data;
}

const string & DSAuth::getQueryData() const{
    return this->queryData;
}

void DSAuth::setBrotherData(const string & data){
    this->brothreData = data;
}

const string & DSAuth::getBrotherData() const{
    return this->brothreData;
}


void DSAuth::setSiblingPath(string * path){
    this->siblingPath = path;
}

string * DSAuth::getSiblingPath() const {
    return this->siblingPath;
}
