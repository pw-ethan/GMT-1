/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: ZZNode.cpp
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

#include "ZZNode.h"

using namespace std;

ZZNode::ZZNode(const ZZ &wgt){
    this->weight = wgt;
    this->parent = NULL;
    this->leftChild = NULL;
    this->rightChild = NULL;
}

ZZNode::~ZZNode(){
    //cout << "~Node()" << endl;
}

ZZ ZZNode::getWeight(){
    return this->weight;
}
/*
void Node::setID(const uint16_t & id){
    this->id = id;
}*/

ZZNode * ZZNode:: getParent(){
    return this->parent;
}

void ZZNode::setParent(ZZNode *node){
    this->parent = node;
}

ZZNode * ZZNode::getLeftChild(){
    return this->leftChild;
}

void ZZNode::setLeftChild(ZZNode * node){
    this->leftChild = node;
}

ZZNode * ZZNode::getRightChild(){
    return this->rightChild;
}

void ZZNode::setRightChild(ZZNode * node){
    this->rightChild = node;
}
