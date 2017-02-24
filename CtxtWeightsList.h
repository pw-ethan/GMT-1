/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: CtxtWeightsList.h
 *  简要描述:.
 *
 *  创建日期: 2017年02月23日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef CTXTWEIGHTSLIST_H
#define CTXTWEIGHTSLIST_H

#include <NTL/ZZ.h>
#include <string>
#include "./libs/fhe/FHE.h"

using namespace NTL;

struct CtxtWeightsList
{
    CtxtWeightsList(const FHEPubKey* pubkey, const int& num);
    virtual ~CtxtWeightsList();

    Ctxt* cweights;
    int num;
};


#endif

