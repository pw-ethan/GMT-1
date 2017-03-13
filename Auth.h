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


using namespace NTL;

struct CtxtLinkListNode
{
    CtxtLinkListNode():pweight(NULL), next(NULL) {}
    CtxtLinkListNode(Ctxt *p):pweight(p), next(NULL) {}
    Ctxt *pweight;
    CtxtLinkListNode *next;
};

struct Auth{
    Auth(int n, const ZZ &qvalue, const ZZ &bvalue);

    int num;
    ZZ queryValue;
    ZZ brotherValue;
    CtxtLinkListNode *hdr;
};


#endif
