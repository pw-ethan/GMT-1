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
#pragma once

#include <NTL/ZZ.h>
#include <stdint.h>
#include <string>

#include "Node.h"
#include "DBUtility.h"

using namespace NTL;
using namespace std;

class VTree
{
public:
    VTree();
    virtual ~VTree();

    /* update Verifier's weights tree */
    bool updateVTree(const ZZ * weights, const uint16_t &numAdd2Weights);
    /* add value and update evidence */
    bool addValue(const ZZ & value);
    /* Traversing the weights tree by level */
    void printVTree();

    ZZ getEvidence();
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
    Node * root;            // the root of Verifier's weights tree
    uint16_t maxElems;
    DBUtility *db;

private:
    void deleteTree(Node * root);
    /* Get the next insertable position in the weights tree of Verifier */
    Node * getPosition(Node * root);
    /* Pre-order traversal binary tree */
    //void PreOrderBiTree(Node * root);
public:
    string ZZ2Bytes(const ZZ & x);
    ZZ Bytes2ZZ(const string & x);
};
