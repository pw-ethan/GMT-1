/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: ZZNode.h
 *  简要描述: 广义Merkle Tree的节点
 *
 *  创建日期: 2016年10月13日
 *  作者: pw-ethan
 *  说明: 声明了广义Merkle Tree节点的数据结构以及方法
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/
#ifndef ZZ_NODE_H
#define ZZ_NODE_H

#include <NTL/ZZ.h>

using namespace NTL;

class ZZNode
{
public:
    ZZNode(const ZZ &wgt);
    virtual ~ZZNode();

    ZZ getWeight();
    ZZNode *getParent();
    void setParent(ZZNode * node);
    ZZNode *getLeftChild();
    void setLeftChild(ZZNode * node);
    ZZNode *getRightChild();
    void setRightChild(ZZNode * node);

private:
    ZZ weight;
    ZZNode *parent;
    ZZNode *leftChild;
    ZZNode *rightChild;
};

#endif
