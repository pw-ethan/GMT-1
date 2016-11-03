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
#pragma once

#include <string>
#include <stdint.h>
#include "./libs/fhe/FHE.h"

#include "Node.h"
#include "CryptoUtility.h"

class PTree
{
public:
    PTree();
    virtual ~PTree();
    
    /* update Prover's weights tree and values tree */
    void updatePTree(const uint16_t * ids, const uint16_t & numAdd2Weights);
    /* Traversing the weights tree by level */
    void printPTree();

private:
    uint16_t depth;
    uint16_t numElems;
    uint16_t maxElems;
    Node * rootWeights;
    Node * rootValues;

private:
    Node * getPosition(Node * root);
    void deleteTree(Node * root);
    void printTree(Node * root);
public:
    string Ctxt2Bytes(Ctxt * src);
    Ctxt * Bytes2Ctxt(const string & x);
};

