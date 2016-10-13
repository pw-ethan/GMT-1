/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: Node.cpp
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

#include "Node.h"

using namespace std;


Node::Node(int16_t id, Node * parent, Node * leftChild, Node * rightChild){
    this->id = id;
    this->parent = parent;
    this->leftChild = leftChild;
    this->rightChild = rightChild;
}

Node::~Node(){
    
}

int16_t Node::getID(){
    return this->id;
}

void Node::setID(uint16_t id){
    this->id = id;
}

Node * Node:: getParent(){
    return this->parent;
}

void Node::setParent(Node *node){
    this->parent = node;
}

Node * Node::getLeftChild(){
    return this->leftChild;
}

void Node::setLeftChild(Node * node){
    this->leftChild = node;
}

Node * Node::getRightChild(){
    return this->rightChild;
}

void Node::setRightChild(Node * node){
    this->rightChild = node;
}
