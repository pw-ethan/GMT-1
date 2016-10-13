/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: vTree.h
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
#include "Node.h"

using namespace NTL;

class vTree
{
public:
    vTree(const uint16_t depth=4);
    virtual ~vTree();

    ZZ getEvidence();
    void setEvidence(const ZZ & value);
    uint16_t getDepth();
    void setDepth(const uint16_t & depth);
    uint16_t getNumElems();
    void setNumElems(const uint16_t & numElems);
    Node * getRoot();
    

private:
    ZZ evidence;
    uint16_t depth;
    uint16_t numElems;
    Node * root;

private:
    void deleteTree(Node * root);
};
