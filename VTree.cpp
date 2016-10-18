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
#include <string.h>

#include "VTree.h"
#include "common.h"
#include "Base64.h"
#include "config.h"

#include "memory_dump.h"

VTree::VTree(){
    this->depth = 0;
    this->maxElems = 0;
    clear(this->evidence);
    this->numElems = 0;
    this->root = new Node(0);
    this->db = new DBUtility();
    this->db->initDB(HOST, USER, PWD, DB_NAME);
}

VTree::~VTree(){
    this->db->deleteDB();
    delete this->db;
    deleteTree(this->root);
}

bool VTree::updateVTree(const ZZ * weights, const uint16_t & numAdd2Weights){
    /*if(this->maxElems != this->numElems){
        cerr << "[Error] VTree::updateVTree() -- number to add to Weights is NOT OK!"<< endl;
        return;
    }*/

    this->maxElems *= 2;
    this->depth += 1;

    this->db->startSQL();
    bool _return = true;
    uint16_t ids[numAdd2Weights];
    for(uint16_t i = 0; i < numAdd2Weights; i++){
        ids[i] = numAdd2Weights + i;
        string stmp = this->ZZ2Bytes(weights[i]);
        if(!(this->db->insertDB("weights", ids[i], stmp))){
            _return = false;
            break;
        }
    }
    this->db->endSQL(_return);
    if(_return == false){
        this->maxElems /= 2;
        this->depth -= 1;
        return false;
    }

    // update evidence
    this->evidence *= weights[0];

    // update root
    Node * node = new Node(ids[0]);
    node->setLeftChild(this->root);
    this->root->setParent(node);
    this->root = node;
    if(numAdd2Weights == 1){
        cout << "[Info] VTree::updateVTree() -- numAdd2Weights is 1" << endl;
        cout << endl;
        this->maxElems = 1;
        this->root->setLeftChild(NULL);
        return true;
    }

    // update right child tree
    node = new Node(ids[1]);
    this->root->setRightChild(node);
    node->setParent(this->root);

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
    return _return;
}

bool VTree::addValue(const ZZ & value){
    bool _return = true;
    uint16_t offset = this->numElems;
    uint16_t numOfCorner = this->depth - 1;

    bool LeftOrRight[numOfCorner];
    for(uint16_t i = 0; i < numOfCorner; i++){
        if(offset % 2 == 1){
            LeftOrRight[numOfCorner - i - 1] = true;
        }else{
            LeftOrRight[numOfCorner - i - 1] = false;
        }
        offset /= 2;
    }

    ZZ valueAdd2Evidence = value;
    Node * point = this->root;
    this->db->startSQL();
    for(uint16_t i = 0; i < numOfCorner; i++){
        if(LeftOrRight[i]){
            point = point->getRightChild();
        }else{
            point = point->getLeftChild();
        }

        string result = this->db->queryDB("weights", point->getID());
        if(result.empty()){
            cerr << "[Info] VTree::addValue() -- Geting ZZ from DB is NOT OK" << endl;
            _return = false;
            break;
        }
        valueAdd2Evidence *= Bytes2ZZ(result);
    }
    string result = this->db->queryDB("weights", this->root->getID());
    if(result.empty()){
        cerr << "[Info] VTree::addValue() -- Geting ZZ from DB is NOT OK" << endl;
        _return = false;
    }
    this->db->endSQL(_return);
    if(_return){
        this->evidence += valueAdd2Evidence;
        this->numElems += 1;
    }
    return _return;
}

void VTree::printVTree(){
    vector<Node *> vec;
    vec.push_back(this->root);
    uint16_t cur = 0;
    uint16_t last = 1;
    cout << "[Info] the weights tree structure is as follows..." << endl;
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

uint16_t VTree::getDepth(){
    return this->depth;
}
/*
void VTree::setDepth(const uint16_t & depth){
    this->depth = depth;
}*/

uint16_t VTree::getNumElems(){
    return this->numElems;
}

uint16_t VTree::getMaxElems(){
    return this->maxElems;
}
/*
void VTree::setNumElems(const uint16_t & numElems){
    this->numElems = numElems;
}*/
/*
Node * VTree::getRoot(){
    return this->root;
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

string VTree::ZZ2Bytes(const ZZ & x){
    unsigned char pstr[sizeof(ZZ)]; // size of ZZ is 8
    BytesFromZZ(pstr, x, sizeof(ZZ));
    string _return = base64_encode(pstr, sizeof(ZZ));
    return _return;
}

ZZ VTree::Bytes2ZZ(const string & x){
    string y = base64_decode(x);
    ZZ _return = ZZFromBytes((const unsigned char *)(y.c_str()), sizeof(ZZ));
    return _return;
}

