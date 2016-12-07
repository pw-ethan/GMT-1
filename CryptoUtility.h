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

#ifndef CRYPTOUTILITY_H
#define CRYPTOUTILITY_H

#include "./libs/fhe/FHE.h"

class CryptoUtility {

public:
    CryptoUtility();
    virtual ~CryptoUtility();

    void initFHE();
    void initFHEByVerifier();
    void initFHEByProver();
    
    Ctxt * encrypt(const ZZ & plaintext);
    ZZX * decrypt(const Ctxt & ciphertext);

public:
    string Ctxt2Bytes(const Ctxt & x);
    Ctxt * Bytes2Ctxt(const string & x);

    /*
    FHEcontext * getContext();
    FHESecKey *getSecKey();
    FHEPubKey *getPubKey();
*/
private:
    FHEcontext *context;
    FHEPubKey *pubkey;
    FHESecKey *seckey;
};

#endif
