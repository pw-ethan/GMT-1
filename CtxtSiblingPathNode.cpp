/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: CtxtSiblingPathNode.cpp
 *  简要描述: 广义Merkle Tree的节点
 *
 *  创建日期: 2016年10月13日
 *  作者: pw-ethan
 *  说明: 广义Merkle Tree节点的实现
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include <iostream>

#include "CtxtSiblingPathNode.h"

using namespace std;

CtxtSiblingPathNode::CtxtSiblingPathNode(FHEPubKey *pubkey){
    this->weight = new Ctxt(*pubkey);
    this->next = NULL;
}

CtxtSiblingPathNode::~CtxtSiblingPathNode(){

}

Ctxt * CtxtSiblingPathNode::getWeight(){
    return this->weight;
}

void CtxtSiblingPathNode::setWeight(Ctxt * wgt){
    this->weight = wgt;
}


CtxtSiblingPathNode * CtxtSiblingPathNode:: getNext(){
    return this->next;
}

void CtxtSiblingPathNode::setNext(CtxtSiblingPathNode *node){
    this->next = node;
}
