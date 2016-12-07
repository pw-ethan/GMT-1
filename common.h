/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: common.h
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

#pragma once

#include <stdint.h>
#include <NTL/ZZ.h>
#include <time.h>
#include "./libs/fhe/FHE.h"

using namespace NTL;
using namespace std;

/**
 *  Calculate the nth power of 2
 */
uint16_t power_two(uint16_t n);

ZZ genRandomValue();

bool isItTime();
//string toBytes(void* src, int len);

//double getDouble(const string &x);

//Ctxt* getCtxt(const string &x);

