/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: VTree.cpp
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
#include <queue>
#include <vector>
#include <stdlib.h>
#include <string.h>

#include "VTree.h"
#include "common.h"
#include "Base64.h"

VTree::VTree(){
    this->depth = 0;
    this->maxElems = 0;
    clear(this->evidence);
    this->numElems = 0;
    this->root = new Node(0);
}

VTree::~VTree(){
    deleteTree(this->root);
}

void VTree::updateVTree(const uint16_t * ids, const uint16_t numAdd2Weights){
    if(numAdd2Weights != power_two(this->depth)){
        cerr << "[Error] VTree::updateVTree() -- numAdd2Weights is NOT OK!"<< endl;
        return;
    }
    maxElems *= 2;
    depth += 1;

    // update root
    Node * node = new Node(ids[0]);
    node->setLeftChild(this->root);
    this->root->setParent(node);
    root = node;
    
    if(numAdd2Weights == 1){
        cout << "[Info] VTree::updateVTree() -- numAdd2Weights is 1" << endl;
        root->setLeftChild(NULL);
        return;
    }
    node = new Node(ids[1]);
    root->setRightChild(node);
    node->setParent(root);

    for(uint16_t i = 2; i < numAdd2Weights; i++){
        Node * position = getPosition(node);
        Node * tmp = new Node(ids[i]);
        if(i % 2 == 1){
            position->setRightChild(tmp);
        }else{
            position->setLeftChild(tmp);
        }
        tmp->setParent(position);
    }

}

void VTree::printVTree(){
    vector<Node *> vec;
    vec.push_back(this->root);
    uint16_t cur = 0;
    uint16_t last = 1;
    cout << "the weights tree structure is as follows..." << endl;
    while(cur < vec.size()){
        last = vec.size();
        while(cur < last){
            cout << vec[cur]->getID() << " ";
            if(vec[cur]->getLeftChild()){
                vec.push_back(vec[cur]->getLeftChild());
            }
            if(vec[cur]->getRightChild()){
                vec.push_back(vec[cur]->getRightChild());
            }
            cur++;
        }
        cout << endl;
    }

    cout << endl;
    vector<Node *>().swap(vec);
}


ZZ VTree::getEvidence(){
    return this->evidence;
}
/*
void VTree::setEvidence(const ZZ & value){
    this->evidence = value;
}*/
/*
uint16_t VTree::getDepth(){
    return this->depth;
}*/
/*
void VTree::setDepth(const uint16_t & depth){
    this->depth = depth;
}*/
/*
uint16_t VTree::getNumElems(){
    return this->numElems;
}*/
/*
void VTree::setNumElems(const uint16_t & numElems){
    this->numElems = numElems;
}*/
/*
Node * VTree::getRoot(){
    return this->root;
}*/
/*
void VTree::insertWeight(const uint16_t & id, const ZZ & value){
    if(this->numElems < this->maxElems){
        cout << "insert #" << this->numElems << ": {"<< id << ", "<< value << "}" << endl;

        this->numElems += 1;
    }
}*/

void VTree::deleteTree(Node *root){
    if(root == NULL){
        return;
    }else{
        deleteTree(root->getLeftChild());
        deleteTree(root->getRightChild());
        delete root;
    }
}

Node * VTree::getPosition(Node * root){
    queue<Node *> q;
    q.push(root);
    do{
        Node * node = q.front();
        if(node->getLeftChild() == NULL || node->getRightChild() == NULL){
            return node;
        }else{
            q.pop();
            q.push(node->getLeftChild());
            q.push(node->getRightChild());
        }
    }while(!q.empty());
    
    return NULL;
}
/*
void VTree::PreOrderBiTree(Node * root){
    if(root == NULL){
        return;
    }else{
        cout << root->getID() << "  ";
        PreOrderBiTree(root->getLeftChild());
        PreOrderBiTree(root->getRightChild());
    }
}*/

ZZ VTree::Bytes2ZZ(const string & x){
    string y = base64_decode(x);
    //cout << "[Debug] string length: " << x.length() << endl;
    char *p = const_cast<char *>(y.c_str());
    //cout << "[Debug] char * length:" << sizeof(ZZ) << endl;
    ZZ * _return = reinterpret_cast<ZZ *>(p);
    cout << "[Debug] return ZZ:" << *_return << endl;
    return *_return;
}
