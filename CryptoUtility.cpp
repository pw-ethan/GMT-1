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
#include <vector>
#include <cstring>
#include <stdio.h>
#include <unistd.h>

#include "CryptoUtility.h"
#include "Base64.h"
#include "memory_dump.h"

#define BASEFILE "base.key"
#define CONTEXTFILE "context.key"
#define PUBKEYFILE "pubkey.key"
#define SECKEYFILE "seckey.key"

using namespace std;


CryptoUtility::CryptoUtility() {
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

void CryptoUtility::initFHE() {
    std::cout << "[Info] Initializing ..." << std::endl;
    /* initializes parameters */
    m = 0;         // m, p, r, the native plaintext space
    p = 1013;      // p is a prime, 模p计算
    r = 1;
    long L = TREE_LEVELS * 2;         // the number “levels,” i.e. the number of ciphertext primes
    long c = 2;         // number of columns in key switching matrix (recommended c = 2 or c = 3)
    long w = 64;        // the Hamming weight of a secret key (w = 64 recommended)
    long d = 0;
    long k = 80;
    long s = 0;

    m = FindM(k, L, c, p, d, s, 0);

    /* initializes context */
    this->context = new FHEcontext(m, p, r);
    buildModChain(*(this->context), L, c);

    /* generates keys*/
    this->seckey = new FHESecKey(*(this->context));
    this->pubkey = this->seckey;

    this->seckey->GenSecKey(w);

    writeMPR();

    string strContext = FHE2Bytes(this->context, sizeof(FHEcontext));
    write2File(CONTEXTFILE, strContext);

    string strPubKey = FHE2Bytes(this->pubkey, sizeof(FHEPubKey));
    write2File(PUBKEYFILE, strPubKey);

    string strSecKey = FHE2Bytes(this->seckey, sizeof(FHESecKey));
    write2File(SECKEYFILE, strSecKey);
}

void CryptoUtility::initFHEByVerifier(){
    if(access(SECKEYFILE, F_OK) == -1){
        cerr << "[Error] seckey.key does not exist!" << endl;
    }

    initFHEByProver();

    string strSecKey = readFromFile(SECKEYFILE);
    Bytes2FHESecKey(strSecKey);
}

void CryptoUtility::initFHEByProver(){
    if(access(BASEFILE, F_OK) == -1 || access(CONTEXTFILE, F_OK) == -1 || access(PUBKEYFILE, F_OK) == -1){
        cerr << "[Error] base.key or context.key or pubkey.key does not exist!" << endl;
        return;
    }
    
    readMPR();

    cout << "tag 1" << endl;

    string strContext = readFromFile(CONTEXTFILE);
    Bytes2FHEContext(strContext);
    
    cout << "tag 2" << endl;

    string strPubKey = readFromFile(PUBKEYFILE);
    Bytes2FHEPubKey(strPubKey);
}

FHEPubKey *CryptoUtility::getPubKey() {
    return pubkey;
}

FHESecKey *CryptoUtility::getSecKey() {
    return seckey;
}

void CryptoUtility::write2File(const char * path, const string & content){
    ofstream out(path, ios::out|ios::binary);
    out << content;
    out.close();
}

string CryptoUtility::readFromFile(const char * path){
    ifstream in(path, ios::in|ios::binary);
    string _return;
    in >> _return;
    in.close();
    return _return;
}

FHEcontext * CryptoUtility::getContext(){
    return this->context;
}

string CryptoUtility::FHE2Bytes(void * src, unsigned int len){
    unsigned char pstr[len];
    memcpy(pstr, src, len);
    string _return = base64_encode(pstr, len);
    return _return;
}

void CryptoUtility::Bytes2FHEContext(const string & x){
    string y = base64_decode(x);
    this->context = new FHEcontext(this->m, this->p, this->r);
    memcpy(this->context, y.c_str(), sizeof(FHEcontext));
}

void CryptoUtility::Bytes2FHEPubKey(const string & x){
    string y = base64_decode(x);
    this->pubkey = new FHESecKey(*(this->context));
    memcpy(this->pubkey, y.c_str(), sizeof(FHEPubKey));
}

void CryptoUtility::Bytes2FHESecKey(const string & x){
    string y = base64_decode(x);
    this->seckey = new FHESecKey(*(this->context));
    memcpy(this->seckey, y.c_str(), sizeof(FHESecKey));
}

void CryptoUtility::writeMPR(){
    ofstream out(BASEFILE, ios::out);
    out << this->m << endl;
    out << this->p << endl;
    out << this->r << endl;
    out.close();
}

void CryptoUtility::readMPR(){
    ifstream in(BASEFILE, ios::in);
    in >> this->m;
    in >> this->p;
    in >> this->r;
    in.close();
}
