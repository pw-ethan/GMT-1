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

#include "CryptoUtility.h"

#include <fstream>
#include <sstream>
#include <vector>

#include <unistd.h>

#include "config.h"

#define CONTEXTFILE "context.key"
#define PUBKEYFILE "pubkey.key"
#define SECKEYFILE "seckey.key"


CryptoUtility::CryptoUtility() {
    context = NULL;
    pubkey = NULL;
    seckey = NULL;
}

CryptoUtility::~CryptoUtility() {
}

void CryptoUtility::initFHE() {
    cout << "[Info] Initializing ..." << endl;
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
    seckey = new FHESecKey(*context);
    seckey->GenSecKey(w);

    pubkey = seckey;

    // write FHEcontext/FHEPubKey/FHESecKey to file
    ofstream osc(CONTEXTFILE, ios::binary);
    if(osc.is_open()){
        writeContextBase(osc, *context);
        osc << *context;
        osc.close();
    }

    ofstream osp(PUBKEYFILE, ios::binary);
    if(osp.is_open()){
        osp << *pubkey;
        osp.close();
    }

    ofstream oss(SECKEYFILE, ios::binary);
    if(oss.is_open()){
        oss << *seckey;
        oss.close();
    }

    cout << "[Info] Initializing OK" << endl;
}

void CryptoUtility::initFHEByVerifier(){
    cout << "[Info] Initializing by Verifier ..." << endl;
    if(access(SECKEYFILE, F_OK) == -1){
        cerr << "[Error] seckey.key does not exist!" << endl;
        return;
    }

    if(access(CONTEXTFILE, F_OK) == -1 || access(PUBKEYFILE, F_OK) == -1){
        cerr << "[Error] context.key or pubkey.key does not exist!" << endl;
        return;
    }

    vector<long> gens, ords;
    unsigned long m, p, r;
    ifstream isc(CONTEXTFILE, ios::binary);
    if(isc.is_open()){
        readContextBase(isc, m, p, r, gens, ords);
        context = new FHEcontext(m, p, r, gens, ords);
        isc >> *context;
        isc.close();
    }

    ifstream isp(PUBKEYFILE, ios::binary);
    if(isp.is_open()){
        pubkey = new FHEPubKey(*context);
        isp >> *pubkey;
        isp.close();
    }
    
    ifstream iss(SECKEYFILE, ios::binary);
    if(iss.is_open()){
        seckey = new FHESecKey(*context);
        iss >> *seckey;
        iss.close();
    }

    cout << "[Info] Initializing OK" << endl;
}

void CryptoUtility::initFHEByProver(){
    cout << "[Info] Initializing by Prover ..." << endl;
    if(access(CONTEXTFILE, F_OK) == -1 || access(PUBKEYFILE, F_OK) == -1){
        cerr << "[Error] context.key or pubkey.key does not exist!" << endl;
        return;
    }

    vector<long> gens, ords;
    unsigned long m, p, r;
    ifstream isc(CONTEXTFILE, ios::binary);
    if(isc.is_open()){
        readContextBase(isc, m, p, r, gens, ords);
        context = new FHEcontext(m, p, r, gens, ords);
        isc >> *context;
        isc.close();
    }

    ifstream isp(PUBKEYFILE, ios::binary);
    if(isp.is_open()){
        pubkey = new FHEPubKey(*context);
        isp >> *pubkey;
        isp.close();
    }

    cout << "[Info] Initializing OK" << endl;
}

Ctxt * CryptoUtility::encrypt(const ZZ & plaintext){
    if(pubkey == NULL){
        cout << "[Error] CryptoUtility::encrypt() -- pubkey is NOT OK!" << endl;
        return NULL;
    }
    Ctxt * ciphertext = new Ctxt(*pubkey);
    pubkey->Encrypt(*ciphertext, NTL::to_ZZX(plaintext));
    return ciphertext;
}

ZZX * CryptoUtility::decrypt(const Ctxt & ciphertext){
    if(seckey == NULL){
        cout << "[Error] CryptoUtility::decrypt() -- seckey is NOT OK!" << endl;
        return NULL;
    }
    ZZX * plaintext = new ZZX();
    seckey->Decrypt(*plaintext, ciphertext);
    return plaintext;
}

string CryptoUtility::Ctxt2Bytes(const Ctxt & x){
    stringstream ss;
    ss << x;
    string _return = ss.str();
    return _return;
}

Ctxt * CryptoUtility::Bytes2Ctxt(const string & x){
    Ctxt * _return = new Ctxt(*pubkey);
    stringstream ss(x);
    ss >> *_return;
    return _return;
}

/*
FHEPubKey *CryptoUtility::getPubKey() {
    return pubkey;
}

FHESecKey *CryptoUtility::getSecKey() {
    return seckey;
}

FHEcontext * CryptoUtility::getContext(){
    return this->context;
}
*/
