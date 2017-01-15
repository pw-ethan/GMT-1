/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: Auth.cpp
 *  简要描述: 
 *
 *  创建日期: 2017年01月14日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include "Auth.h"

#include <iostream>

using namespace std;

Auth::Auth(){}
Auth::~Auth(){}

uint16_t Auth::getNum()
{
    return this->num;
}

void Auth::setNum(const uint16_t& n)
{
    this->num = n;
}

ZZ Auth::getQueryValue()
{
    return this->queryValue;
}

void Auth::setQueryValue(const ZZ& value)
{
    this->queryValue = value;
}

ZZ Auth::getBrotherValue()
{
    return this->brotherValue;
}

void Auth::setBrotherValue(const ZZ& value)
{
    this->brotherValue = value;
}

CtxtSiblingPathNode * Auth::getSiblingPath()
{
    return this->siblingPath;
}

void Auth::setSiblingPath(CtxtSiblingPathNode *node)
{
    this->siblingPath = node;
}


