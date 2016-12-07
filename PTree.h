/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: PTree.h
 *  简要描述: GMT-1方案的Prover的树形结构
 *
 *  创建日期: 2016年10月18日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef PTREE_H
#define PTREE_H

#include <string>
#include <stdint.h>
#include "./libs/fhe/FHE.h"

#include "Node.h"
#include "CryptoUtility.h"
#include "DBUtility.h"
#include "DSAuth.h"

class PTree
{
public:
    PTree();
    virtual ~PTree();

    /* update Prover's weights tree and values tree */
    bool updatePTree(const string * strWeights, const uint16_t & numAdd2Weights);
    /* add value */
    bool addValue(const ZZ & value);
    /* query value */
    string queryValue(const uint16_t & index);
    /* Traversing the weights tree by level */
    void printPTree();
    bool isFull();
    uint16_t getMaxElems();
    uint16_t getNumElems();
    uint16_t getDepth();

    string test();

private:
    uint16_t depth;
    uint16_t numElems;
    uint16_t maxElems;
    Node * rootWeights;
    Node * rootValues;
    DBUtility * db;
    CryptoUtility * cy;

private:
    Node * getPosition(Node * root);
    void deleteTree(Node * root);
    void printTree(Node * root);
    string ZZ2Bytes(const ZZ & x);
    ZZ Bytes2ZZ(const string & x);
};

#endif
