/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: VTree.h
 *  简要描述:.GMT-1方案的Verifier的树形结构
 *
 *  创建日期: 2016年10月13日
 *  作者: pw-ethan
 *  说明:.广义Merkle Tree的声明
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#ifndef VTREE_H
#define VTREE_H

#include <NTL/ZZ.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "./libs/fhe/FHE.h"

#include "ZZNode.h"
//#include "DBUtility.h"
#include "CryptoUtility.h"
#include "DSAuth.h"
#include "Auth.h"
#include "AuthVerify.h"
#include "CtxtSiblingPathNode.h"
#include "CtxtWeightsList.h"

using namespace NTL;
using namespace std;

class VTree
{
public:
    VTree();
    virtual ~VTree();

    /* update Verifier's weights tree */
    bool updateVTree(const ZZ * weights, const uint16_t &numAdd2Weights);
    /* Add value and update evidence */
    bool addValue(const ZZ & value);
    /* Traversing the weights tree by level */
    void printVTree();
    /* Verify query result using DSAuth */
    bool verify(const uint16_t index, AuthVerify & auth);

    AuthVerify Str2AuthVerify(const string &str);

    ZZ getEvidence();
    
    // check whether the tree is full or not
    bool isFull();
    // get the number to add weights tree
    uint16_t getNumAdd2Weights();
    // generate weights(ZZ type)
    ZZ * genWeights(const int num);

    void genCtxtWeights(const ZZ *weights, uint16_t numAdd2Weights,CtxtSiblingPathNode* pweights);

    CtxtWeightsList* genCtxtWeights(const ZZ *weights, uint16_t numAdd2Weights);

    string CtxtWeights2Str(const CtxtWeightsList *cwlist);

    // encrypt weights and publish them
    void weights2Str(const ZZ * weigths, string * strWeights, uint16_t numAdd2Weights);
    //void setEvidence(const ZZ & value);
    uint16_t getDepth();
    //void setDepth(const uint16_t & depth);
    uint16_t getNumElems();
    uint16_t getMaxElems();
    //void setNumElems(const uint16_t & numElems);
    //Node * getRoot();

private:
    ZZ evidence;
    uint16_t depth;
    uint16_t numElems;
    ZZNode * root;            // the root of Verifier's weights tree
    uint16_t maxElems;
    //DBUtility *db;
public:
    CryptoUtility *cy;

private:
    void deleteTree(ZZNode * root);
    /* Get the next insertable position in the weights tree of Verifier */
    ZZNode * getPosition(ZZNode * root);
    /* Pre-order traversal binary tree */
    //void PreOrderBiTree(Node * root);
public:
    string ZZ2Bytes(const ZZ & x);
    ZZ Bytes2ZZ(const string & x);
    
    string test();
    void test(Ctxt* tmp);
};

#endif
