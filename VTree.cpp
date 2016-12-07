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

#define VHOST "localhost"
#define VUSER "root"
#define VPWD "1234"
#define VDB_NAME "gmt_1_v"

VTree::VTree(){
    this->depth = 0;
    this->maxElems = 0;
    clear(this->evidence);
    this->numElems = 0;
    this->root = new Node(0);
    this->db = new DBUtility();
    this->db->initDB(VHOST, VUSER, VPWD, VDB_NAME);
    this->cy = new CryptoUtility();
    this->cy->initFHEByVerifier();
}

VTree::~VTree(){
    delete this->cy;
    this->db->deleteDB("weights");
    delete this->db;
    deleteTree(this->root);
}

bool VTree::updateVTree(const ZZ * weights, const uint16_t & numAdd2Weights){
    cout << "[Info] update VTree" << endl;

    if(numAdd2Weights != power_two(this->depth)){
        //cerr << "[Error] VTree::updateVTree() -- number of adding to Weights is NOT OK!" << endl;
        LOGERROR("[Error] VTree::updateVTree() -- number of adding to Weights is NOT OK!");
        return false;
    }

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
        //cout << "[Info] VTree::updateVTree() -- numAdd2Weights is 1" << endl;
        LOGINFO("[Info] VTree::updateVTree() -- numAdd2Weights is 1");
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
            //cerr << "[Error] VTree::addValue() -- Getting ZZ from DB is NOT OK" << endl;
            LOGERROR("[Error] VTree::addValue() -- Getting ZZ from DB is NOT OK!");
            _return = false;
            break;
        }
        valueAdd2Evidence *= Bytes2ZZ(result);
    }
    string result = this->db->queryDB("weights", this->root->getID());
    if(result.empty()){
        //cerr << "[Info] VTree::addValue() -- Geting ZZ from DB is NOT OK" << endl;
        LOGERROR("[Error] VTree::addValue() -- Getting ZZ from DB is NOT OK!");
        _return = false;
    }
    valueAdd2Evidence *= Bytes2ZZ(result);
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

bool VTree::verify(const uint16_t index, const string & result){
    bool _return = true;

    uint16_t offset = index;
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
    Node * pointOfWeights = this->root;
    for(uint16_t i = 0; i < numOfCorner; i++){
        if(LeftOrRight[i]){
            pointOfWeights = pointOfWeights->getRightChild();
        }else{
            pointOfWeights = pointOfWeights->getLeftChild();
        }
    }

    DSAuth ds;
    ds.fromString(result);

    cout << "[Info] query value-" << index << " : " << Bytes2ZZ(ds.getSomething("query-value")) << endl;
    //string * siblingPath = ds.getSiblingPath();
    ZZX master = to_ZZX(Bytes2ZZ(ds.getSomething("query-value")));
    ZZX slave = to_ZZX(Bytes2ZZ(ds.getSomething("brother-value")));
    ZZX master_weight;
    ZZX slave_weight;

    offset = index;

    this->db->startSQL();

    string str_mw = this->db->queryDB("weights", pointOfWeights->getID());
    if(str_mw.empty()){
        //cerr << "[Error] VTree::queryValue() -- Getting value from DB is NOT OK" << endl;
        LOGERROR("[Error] VTree::queryValue() -- Getting value from DB is NOT OK!");
        _return = false;
    }
    master_weight = Bytes2ZZ(str_mw);

    //cout << endl << endl << "master_weight-" << master_weight << endl;

    string str_sw;
    if(offset % 2 == 1){
        str_sw = this->db->queryDB("weights", pointOfWeights->getParent()->getLeftChild()->getID());
    }else{
        str_sw = this->db->queryDB("weights", pointOfWeights->getParent()->getRightChild()->getID());
    }
    if(str_sw.empty()){
        //cerr << "[Error] VTree::queryValue() -- Getting value from DB is NOT OK" << endl;
        LOGERROR("[Error] VTree::queryValue() -- Getting value from DB is NOT OK!");
        _return = false;
    }
    slave_weight = Bytes2ZZ(str_sw);

    //cout << "slave_weight-" << slave_weight << endl;

    master = master * master_weight + slave * slave_weight;

    //cout << "master-" << master << endl;

    pointOfWeights = pointOfWeights->getParent();
    offset /= 2;

    uint16_t num = stoi(ds.getSomething("num"));
    for(uint16_t i = 0; i < num; i++){
        Ctxt * ctmp = this->cy->Bytes2Ctxt(ds.getSomething("sibling-path-" + to_string(i)));
        slave = *(this->cy->decrypt(*ctmp));

        //cout << "slave-" << slave << endl;

        string str_mwt = this->db->queryDB("weights", pointOfWeights->getID());
        if(str_mwt.empty()){
            //cerr << "[Error] VTree::queryValue() -- Getting value from DB is NOT OK" << endl;
            LOGERROR("[Error] VTree::queryValue() -- Getting value from DB is NOT OK!");
            _return = false;
            break;
        }
        master_weight = Bytes2ZZ(str_mwt);

        //cout << "master_weight-" << master_weight << endl;

        string str_swt;
        if(offset % 2 == 1){
            str_swt = this->db->queryDB("weights", pointOfWeights->getParent()->getLeftChild()->getID());
        }else{
            str_swt = this->db->queryDB("weights", pointOfWeights->getParent()->getRightChild()->getID());
        }
        if(str_swt.empty()){
            //cerr << "[Error] VTree::queryValue() -- Getting value from DB is NOT OK" << endl;
            LOGERROR("[Error] VTree::queryValue() -- Getting value from DB is NOT OK!");
            _return = false;
            break;
        }
        slave_weight = Bytes2ZZ(str_swt);

        //cout << "slave_weight-" << slave_weight << endl;

        master = master * master_weight + slave * slave_weight;

        pointOfWeights = pointOfWeights->getParent();

        offset /= 2;

        //cout << "master-" << master << endl;

    }

    string top = this->db->queryDB("weights", this->root->getID());
    if(top.empty()){
        //cerr << "[Error] VTree::queryValue() -- Getting value from DB is NOT OK" << endl;
        LOGERROR("[Error] VTree::queryValue() -- Getting value from DB is NOT OK!");
        _return = false;
    }

    //cout << "top-" << Bytes2ZZ(top) << endl;

    master *= Bytes2ZZ(top);

    //cout << "master-" << master << endl << endl;

    db->endSQL(_return);

    if(_return && (master[0] % 1013) == evidence % 1013){
        return true;
    }
    return false;
}


ZZ VTree::getEvidence(){
    return this->evidence;
}
/*
void VTree::setEvidence(const ZZ & value){
    this->evidence = value;
}*/

bool VTree::isFull(){
    if(this->numElems == this->maxElems) return true;
    return false;
}

uint16_t VTree::getNumAdd2Weights(){
    return power_two(this->depth);
}

ZZ * VTree::genWeights(const int num){
    
    ZZ *_return = new ZZ[num];
    SetSeed(to_ZZ(time(NULL)));
    for(int i = 0; i < num; i++){
        RandomLen(_return[i], 16);
    }
    /*
    ZZ *_return = new ZZ[num];
    for(int i = 0; i < num; i++){
        _return[i] = to_ZZ(i+num);
    }*/
    return _return;
}

void VTree::weights2Str(const ZZ * weights, string * strWeights, uint16_t numAdd2Weights){
    for(uint16_t i = 0; i < numAdd2Weights; i++){
        strWeights[i] = this->cy->Ctxt2Bytes(*(this->cy->encrypt(weights[i])));
    }
}

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

