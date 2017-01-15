/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: Auth.h
 *  简要描述:.
 *
 *  创建日期: 2017年01月14日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef AUTH_H
#define AUTH_H


#include <NTL/ZZ.h>
#include "./libs/fhe/FHE.h"
#include "stdint.h"
#include "CtxtSiblingPathNode.h"


using namespace NTL;

class Auth{
public:
    Auth();
    virtual ~Auth();

    uint16_t getNum();
    void setNum(const uint16_t& n);
    ZZ getQueryValue();
    void setQueryValue(const ZZ& value);
    ZZ getBrotherValue();
    void setBrotherValue(const ZZ& value);
    CtxtSiblingPathNode * getSiblingPath();
    void setSiblingPath(CtxtSiblingPathNode *node);

private:
    uint16_t num;
    ZZ queryValue;
    ZZ brotherValue;
    CtxtSiblingPathNode *siblingPath;

};


#endif

