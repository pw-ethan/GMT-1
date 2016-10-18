/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: CryptoUtility.cpp
 *  简要描述: GMT-1方案使用的全同态加解密算法
 *
 *  创建日期: 2016年10月18日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include <fstream>

#include "CryptoUtility.h"


CryptoUtility::CryptoUtility() {
    init_fhe();
}

CryptoUtility::~CryptoUtility() {
    /*
    std::cout << "~CryptoUtility()" << std::endl;
    if(context != NULL){
        delete context;
    }
    if(publicKey != NULL){
        delete publicKey;
    }
    if(secretKey != NULL){
        delete secretKey;
    }
    std::cout << "ok" << std::endl;
    */
}

void CryptoUtility::init_fhe() {
    std::cout << "[Info] Initializing ..." << std::endl;
    /* initializes parameters */
    long m = 0;         // m, p, r, the native plaintext space
    long p = 1013;      // p is a prime, 模p计算
    long r = 1;
    long L = TREE_LEVELS * 2;         // the number “levels,” i.e. the number of ciphertext primes
    long c = 2;         // number of columns in key switching matrix (recommended c = 2 or c = 3)
    long w = 64;        // the Hamming weight of a secret key (w = 64 recommended)
    long d = 0;
    long k = 80;
    long s = 0;

    m = FindM(k, L, c, p, d, s, 0);

    /* initializes context */
    context = new FHEcontext(m, p, r);
    buildModChain(*context, L, c);

    /* generates keys*/
    secretKey = new FHESecKey(*context);
    publicKey = secretKey;

    secretKey->GenSecKey(w);

}

FHEPubKey *CryptoUtility::getPublicKey() {
    return publicKey;
}

FHESecKey *CryptoUtility::getSecretKey() {
    return secretKey;
}

void CryptoUtility::FHEPubKey2File(){
    ofstream out("pubkey.key", ios::out|ios::binary);
    out << *(this->publicKey);
    out.close();
}

FHEPubKey * CryptoUtility::File2FHEPubKey(){
    FHEPubKey * pubkey = new FHESecKey(*context);
    ifstream in("pubkey.key", ios::in|ios::binary);
    in >> *pubkey;
    in.close();
    return pubkey;
}
