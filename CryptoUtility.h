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

#include <string>

#include "./libs/fhe/FHE.h"
#include "config.h"

class CryptoUtility {

public:
    CryptoUtility();
    virtual ~CryptoUtility();

    void initFHE();
    void initFHEByVerifier();
    void initFHEByProver();
    Ctxt * encrypt(const ZZ & plaintext);
    ZZ * decrypt(const Ctxt & ciphertext);

    FHEcontext * getContext();
    FHESecKey *getSecKey();
    FHEPubKey *getPubKey();

    //void FHEPubKey2File();
    //FHEPubKey * File2FHEPubKey();

    //void FHEContext2File();
    //FHEcontext * File2FHEContext();

private:
    FHEcontext *context = NULL;
    FHESecKey *seckey = NULL;
    FHEPubKey *pubkey = NULL;
    long m;
    long p;
    long r;

private:
    void write2File(const char * path, const string & content);
    string readFromFile(const char * path);
    string FHE2Bytes(void * src, unsigned int len);
    void Bytes2FHEContext(const string & x);
    void Bytes2FHEPubKey(const string & x);
    void Bytes2FHESecKey(const string & x);
    void writeMPR();
    void readMPR();

public:
    void writeFHE2File();
    void readFHEFromFile();
};
