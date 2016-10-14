/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: common.cpp
 *  简要描述: 项目中用到的共用方法
 *
 *  创建日期: 2016年8月16日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include "common.h"
#include "Base64.h"

/**
 *  Calculate the nth power of 2
 */
uint16_t power_two(uint16_t n)
{
    return 1<<n;
}

ZZ *gen_weights(const int num)
{
/*
    srand((unsigned)time(NULL));
    int *result = (int *)calloc(num, sizeof(int));
    if(!result){
        return NULL;
    }
    for(int i = 0; i < num; i++){
        result[i] = rand() % WEIGHT_BOUNDARY + 1;
    }
*/
    ZZ *result = new ZZ[num];
    SetSeed(to_ZZ(time(NULL)));
    for(int i = 0; i < num; i++){
        RandomLen(result[i], 16);
    }
    return result;
}

string toBytes(void* src, int len){
    string _return(len, 0);
    char *start = reinterpret_cast<char *>(src);
    for(int i = 0; i < len; i++){
        _return[i] = *(start + i);
    }
    return base64_encode(reinterpret_cast<const unsigned char*>(_return.c_str()), _return.length());
}
/*
double getDouble(const string &x){
    char *p = const_cast<char *>(x.c_str());
    double * _return = 0;
    _return = reinterpret_cast<double *>(p);
    return *_return;
}*/

Ctxt* getCtxt(const string &x){
    char *p = const_cast<char *>(x.c_str());
    Ctxt *_return = NULL;
    _return = reinterpret_cast<Ctxt *>(p);
    return _return;
}
