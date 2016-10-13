/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: vTree.cpp
 *  简要描述: GMT-1方案的Verifier的树形结构
 *
 *  创建日期: 2016年10月13日
 *  作者: pw-ethan
 *  说明: 广义Merkle Tree的定义
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include <iostream>

#include "vTree.h"
#include "common.h"

vTree::vTree(const uint16_t depth){
    this->depth = depth;
    this->maxElems = power_two(depth - 1);
    set(this->evidence);
    this->numElems = 0;
    this->root = new Node(0);
}

vTree::~vTree(){
//    deleteTree(this->root);
}
/*
ZZ vTree::getEvidence(){
    return this->evidence;
}*/
/*
void vTree::setEvidence(const ZZ & value){
    this->evidence = value;
}*/
/*
uint16_t vTree::getDepth(){
    return this->depth;
}*/
/*
void vTree::setDepth(const uint16_t & depth){
    this->depth = depth;
}*/
/*
uint16_t vTree::getNumElems(){
    return this->numElems;
}*/
/*
void vTree::setNumElems(const uint16_t & numElems){
    this->numElems = numElems;
}*/
/*
Node * vTree::getRoot(){
    return this->root;
}*/
/*
void vTree::insertWeight(const uint16_t & id, const ZZ & value){
    if(this->numElems < this->maxElems){
        cout << "insert #" << this->numElems << ": {"<< id << ", "<< value << "}" << endl;

        this->numElems += 1;
    }
}*/
/*
void vTree::deleteTree(Node *root){
    if(root == NULL){
        return;
    }else{
        deleteTree(root->getLeftChild());
        deleteTree(root->getRightChild());
        delete root;
    }
}*/
