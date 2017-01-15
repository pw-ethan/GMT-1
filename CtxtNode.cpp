/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: CtxtNode.cpp
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

#include "CtxtNode.h"

using namespace std;

CtxtNode::CtxtNode(FHEPubKey *pubkey){
    this->weight = new Ctxt(*pubkey);
    this->parent = NULL;
    this->leftChild = NULL;
    this->rightChild = NULL;
}

CtxtNode::~CtxtNode(){
    //cout << "~Node()" << endl;
}

Ctxt* CtxtNode::getWeight(){
    return this->weight;
}

void CtxtNode::setWeight(Ctxt * wgt){
    this->weight = wgt;
}

CtxtNode * CtxtNode::getParent(){
    return this->parent;
}

void CtxtNode::setParent(CtxtNode *node){
    this->parent = node;
}

CtxtNode * CtxtNode::getLeftChild(){
    return this->leftChild;
}

void CtxtNode::setLeftChild(CtxtNode * node){
    this->leftChild = node;
}

CtxtNode * CtxtNode::getRightChild(){
    return this->rightChild;
}

void CtxtNode::setRightChild(CtxtNode * node){
    this->rightChild = node;
}
