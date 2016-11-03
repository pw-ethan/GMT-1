/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: PTree.cpp
 *  简要描述: GMT-1方案的Prover的树形结构
 *
 *  创建日期: 2016年10月18日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include <iostream>
#include <vector>
#include <queue>

#include "PTree.h"
#include "common.h"
#include "Base64.h"

using namespace std;


PTree::PTree(){
    this->depth = 0;
    this->numElems = 0;
    this->maxElems = 0;
    this->rootWeights = new Node(0);
    this->rootValues = new Node(0);
}

PTree::~PTree(){
    deleteTree(this->rootWeights);
    deleteTree(this->rootValues);
}

void PTree::updatePTree(const uint16_t * ids, const uint16_t & numAdd2Weights){
    cout << "[Info] updata PTree" << endl;
    
    if(numAdd2Weights != power_two(this->depth)){
        cerr << "[Error] PTree::updatePTree() -- number of adding to Weights is NOT OK" << endl;
    }

    this->maxElems *= 2;
    this->depth += 1;

    // update root
    Node * node = new Node(ids[0]);
    node->setLeftChild(this->rootWeights);
    this->rootWeights->setParent(node);
    this->rootWeights = node;
    if(numAdd2Weights == 1){
        cout << "[Info] PTree::updatePTree() -- numAdd2Weights is 1" << endl;
        this->maxElems = 1;
        this->rootWeights->setLeftChild(NULL);
        return;
    }

    // update right child tree
    node = new Node(ids[1]);
    this->rootWeights->setRightChild(node);
    node->setParent(this->rootWeights);

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
/*    
    node = new Node(0);
    node->setLeftChild(this->rootValues);
    this->rootValues->setParent(node);
    this->rootValues = node;

    if(numAdd2Weights == 2){
        this->rootValues->setLeftChild(NULL);
        return;
    }

    node = new Node(0);
    this->rootValues->setRightChild(node);
    node->setParent(this->rootValues);

    for(uint16_t i =2; i < numAdd2Weights / 2; i++){
        Node * position = getPosition(node);
        Node * tmp = new Node(0);
        if(i % 2 == 1){
            position->setRightChild(tmp);
        }else{
            position->setLeftChild(tmp);
        }
        tmp->setParent(position);
    }
*/
}

void PTree::printPTree(){
    cout << "[Info] the weights tree structure is as follows..." << endl;
    printTree(this->rootWeights);
//    cout << "[Info] the values tree structure is as follows..." << endl;
//    printTree(this->rootValues);
}

Node * PTree::getPosition(Node * root){
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

void PTree::printTree(Node * root){
    vector<Node *> vec;
    vec.push_back(root);
    uint16_t cur = 0;
    uint16_t last = 0;
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

void PTree::deleteTree(Node * root){
    if(root == NULL){
        return;
    }else{
        deleteTree(root->getLeftChild());
        deleteTree(root->getRightChild());
        delete root;
    }
}

string PTree::Ctxt2Bytes(Ctxt * src){
    unsigned char pstr[sizeof(Ctxt)];
    char * start = reinterpret_cast<char *>(src);
    for(unsigned int i = 0; i < sizeof(Ctxt); i++){
        pstr[i] = *(start + i);
    }
    string _return = base64_encode(pstr, sizeof(Ctxt));
    return _return;
}

Ctxt * PTree::Bytes2Ctxt(const string & x){
    string y = base64_decode(x);
    char * p = const_cast<char *>(y.c_str());
    Ctxt * _return = NULL;
    _return = reinterpret_cast<Ctxt *>(p);
    return _return;
}
