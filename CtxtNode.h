/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: CtxtNode.h
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
#ifndef CTXT_NODE_H
#define CTXT_NODE_H

#include "./libs/fhe/FHE.h"

class CtxtNode
{
public:
    CtxtNode(FHEPubKey *pubkey);
    virtual ~CtxtNode();

    Ctxt* getWeight();
    void setWeight(Ctxt * wgt);
    CtxtNode *getParent();
    void setParent(CtxtNode * node);
    CtxtNode *getLeftChild();
    void setLeftChild(CtxtNode * node);
    CtxtNode *getRightChild();
    void setRightChild(CtxtNode * node);

private:
    Ctxt *weight;
    CtxtNode *parent;
    CtxtNode *leftChild;
    CtxtNode *rightChild;
};

#endif
