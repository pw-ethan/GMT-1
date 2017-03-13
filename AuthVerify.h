/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: AuthVerify.h
 *  简要描述:.
 *
 *  创建日期: 2017年03月13日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef AUTHVERIFY_H
#define AUTHVERIFY_H

#include <NTL/ZZ.h>
#include <vector>
#include "./libs/fhe/FHE.h"

using namespace NTL;
using namespace std;

struct AuthVerify
{
    AuthVerify(int n, const ZZ &qvalue, const ZZ &bvalue):num(n), queryValue(qvalue), brotherValue(bvalue) {}

    int num;
    ZZ queryValue;
    ZZ brotherValue;
    vector<ZZX> innodes;
};

#endif

