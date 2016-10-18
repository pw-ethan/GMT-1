/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: CryptoUtility.h
 *  简要描述:.GMT-1使用的全同态加解密算法
 *
 *  创建日期: 2016年10月18日
 *  作者: pw-ethan
 *  说明:.
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/

#pragma once

#include "./libs/fhe/FHE.h"
#include "config.h"

class CryptoUtility {
private:
    FHEcontext *context;
    FHESecKey *secretKey;
    FHEPubKey *publicKey;

public:
    CryptoUtility();
    virtual ~CryptoUtility();

    void init_fhe();
    FHESecKey *getSecretKey();
    FHEPubKey *getPublicKey();

    void FHEPubKey2File();
    FHEPubKey * File2FHEPubKey();
};
