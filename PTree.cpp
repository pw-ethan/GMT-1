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

#include "PTree.h"

#include <iostream>
#include <vector>
#include <queue>

#include "common.h"
#include "CtxtSiblingPathNode.h"
//#include "Base64.h"

#define PHOST "localhost"
#define PUSER "root"
#define PPWD "1234"
#define PDB_NAME "gmt_1_p"

using namespace std;


PTree::PTree(){
    this->depth = 0;
    this->numElems = 0;
    this->maxElems = 0;
    //this->db = new DBUtility();
    //this->db->initDB(PHOST, PUSER, PPWD, PDB_NAME);
    this->cy = new CryptoUtility();
    //this->cy->initFHEByProver();
    this->cy->initFHEByVerifier();
    this->rootWeights = new CtxtNode(this->cy->getPubKey());
    this->rootValues = new CtxtNode(this->cy->getPubKey());
}

PTree::~PTree(){
    delete this->cy;
    //this->db->deleteDB("weights_p");
    //this->db->deleteDB("values_p");
    //this->db->deleteDB("leaves");
    //delete this->db;
    deleteTree(this->rootWeights);
    deleteTree(this->rootValues);
}

bool PTree::updatePTree(CtxtSiblingPathNode * pweights, const uint16_t & numAdd2Weights){
    cout << "[Info] updata PTree" << endl;

    if(numAdd2Weights != power_two(this->depth)){
        //cerr << "[Error] PTree::updatePTree() -- number of adding to Weights is NOT OK" << endl;
        LOGERROR("[Error] PTree::updatePTree() -- number of adding to Weights is NOT OK!");
        return false;
    }

    this->maxElems *= 2;
    this->depth += 1;

    /*
    this->db->startSQL();
    bool _return = true;
    uint16_t ids[numAdd2Weights];
    for(uint16_t i = 0; i < numAdd2Weights; i++){
        ids[i] = numAdd2Weights + i;
        if(!(this->db->insertDB("weights_p", ids[i], strWeights[i]))){
            _return = false;
            break;
        }
    }
    this->db->endSQL(_return);
    if(_return == false){
        this->maxElems /= 2;
        this->depth -= 1;
        return _return;
    }*/

    // update weights tree
    CtxtSiblingPathNode * pp = pweights->getNext();
    CtxtNode * node = new CtxtNode(this->cy->getPubKey());
    cout << "Tag 1" << endl;
    node->setWeight(pp->getWeight());
    cout << "Tag 2" << endl;
    cout << *(this->cy->decrypt(*(pp->getWeight()))) << endl;
    cout << "Tag 3" << endl;
    //here

    node->setLeftChild(this->rootWeights);
    this->rootWeights->setParent(node);
    this->rootWeights = node;
    if(numAdd2Weights == 1){
        cout << "[Info] PTree::updatePTree() -- numAdd2Weights is 1" << endl;
        //LOGINFO("[Info] PTree::updatePTree() -- numAdd2Weights is 1");
        this->maxElems = 1;
        this->rootWeights->setLeftChild(NULL);
        return true;
    }

    // update right child tree
    pp = pp->getNext();
    node = new CtxtNode(this->cy->getPubKey());
    node->setWeight(pp->getWeight());
    this->rootWeights->setRightChild(node);
    node->setParent(this->rootWeights);
    cout << "Tag.1" << endl;

    for(uint16_t i = 2; i < numAdd2Weights; i++){
        CtxtNode * position = getPosition(node);
        pp = pp->getNext();
        CtxtNode * tmp = new CtxtNode(this->cy->getPubKey());
        tmp->setWeight(pp->getWeight());
        if(i % 2 == 1){
            position->setRightChild(tmp);
        }else{
            position->setLeftChild(tmp);
        }
        tmp->setParent(position);
    }
    cout << "Tag.2" << endl;

    // update values tree
    //string strtmpv = this->cy->Ctxt2Bytes(*(this->cy->encrypt(to_ZZ("0"))));
    //Ctxt * ctmpv = this->cy->encrypt(to_ZZ("0"));
    uint16_t numAdd2Values = numAdd2Weights / 2;
    /*bool _res = true;
    this->db->startSQL();
    uint16_t idst[numAdd2Values];
    for(uint16_t i = 0; i < numAdd2Values; i++){
        idst[i] = numAdd2Values + i;
        if(!(this->db->insertDB("values_p", idst[i], strtmpv))){
            _res = false;
            break;
        }
    }

    string strtopweight = this->db->queryDB("weights_p", this->rootWeights->getLeftChild()->getID());
    Ctxt * topweight = this->cy->Bytes2Ctxt(strtopweight);
    Ctxt * topvalue = NULL;
    if(numAdd2Values == 1){
        string strtopvalue = this->db->queryDB("leaves", 0);
        topvalue = this->cy->encrypt(Bytes2ZZ(strtopvalue));
    }else{
        topvalue = this->cy->Bytes2Ctxt(this->db->queryDB("values_p", this->rootValues->getID()));
    }
    *topvalue *= *topweight;
    this->db->updateDB("values_p", numAdd2Values, this->cy->Ctxt2Bytes(*topvalue));

    this->db->endSQL(_res);*/

    Ctxt * topweight = this->rootWeights->getLeftChild()->getWeight();
    cout << *(this->cy->decrypt(*topweight)) << endl;
    Ctxt * topvalue = this->cy->encrypt(to_ZZ("0"));
    if(numAdd2Values == 1){
        *topvalue += *(this->cy->encrypt(leaves[0]));
        *topvalue *= *topweight;
    }else{
        *topvalue += *(this->rootValues->getWeight());
        *topvalue *= *topweight;
    }
    cout << "Tag.3" << endl;
    //*topvalue *= *topweight;
    cout << "Tag.4" << endl;

    node = new CtxtNode(this->cy->getPubKey());
    node->setWeight(topvalue);
    node->setLeftChild(this->rootValues);
    this->rootValues->setParent(node);
    this->rootValues = node;

    if(numAdd2Values == 1){
        this->rootValues->setLeftChild(NULL);
        return true;
    }

    Ctxt * ctmpv = this->cy->encrypt(to_ZZ("0"));
    node = new CtxtNode(this->cy->getPubKey());
    node->setWeight(ctmpv);
    this->rootValues->setRightChild(node);
    node->setParent(this->rootValues);

    for(uint16_t i =2; i < numAdd2Values; i++){
        CtxtNode * position = getPosition(node);
        Ctxt * ctmpvv = this->cy->encrypt(to_ZZ("0"));
        CtxtNode * tmp = new CtxtNode(this->cy->getPubKey());
        tmp->setWeight(ctmpvv);
        if(i % 2 == 1){
            position->setRightChild(tmp);
        }else{
            position->setLeftChild(tmp);
        }
        tmp->setParent(position);
    }
    return true;
}

bool PTree::addValue(const ZZ & value){
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

    Ctxt * valueAdd2Layer = this->cy->encrypt(value);
    CtxtNode * pointOfWeights = this->rootWeights;
    CtxtNode * pointOfValues = this->rootValues;
    for(uint16_t i = 0; i < numOfCorner; i++){
        if(LeftOrRight[i]){
            pointOfWeights = pointOfWeights->getRightChild();
        }else{
            pointOfWeights = pointOfWeights->getLeftChild();
        }
        if(i < numOfCorner - 1){
            if(LeftOrRight[i]){
                pointOfValues = pointOfValues->getRightChild();
            }else{
                pointOfValues = pointOfValues->getLeftChild();
            }
        }
    }

    //this->db->startSQL();
    for(uint16_t i = 0; i < numOfCorner; i++){
        /*string strw = this->db->queryDB("weights_p", pointOfWeights->getID());
        if(strw.empty()){
            //cerr << "[Error] PTree::addValue() -- Geting weights from DB is NOT OK" << endl;
            LOGERROR("[Error] PTree::addValue() -- Getting weights from DB is NOT OK!");
            _return = false;
            break;
        }
        *valueAdd2Layer *= *(this->cy->Bytes2Ctxt(strw));*/
        *valueAdd2Layer *= *(pointOfWeights->getWeight());
        /*string strv = this->db->queryDB("values_p", pointOfValues->getID());
        if(strv.empty()){
            //cerr << "[Error] PTree::addValue() -- Geting values from DB is NOT OK" << endl;
            LOGERROR("[Error] PTree::addValue() -- Getting values from DB is NOT OK!");
            _return = false;
            break;
        }
        Ctxt * updateValue = this->cy->Bytes2Ctxt(strv);
        *updateValue += *valueAdd2Layer;
        this->db->updateDB("values_p", pointOfValues->getID(), this->cy->Ctxt2Bytes(*updateValue));*/
        *(pointOfValues->getWeight()) += *valueAdd2Layer;

        pointOfWeights = pointOfWeights->getParent();
        pointOfValues = pointOfValues->getParent();
    }
    
    //this->db->insertDB("leaves", this->numElems, ZZ2Bytes(value));
    this->leaves.push_back(value);

    //this->db->endSQL(_return);
    if(_return){
        this->numElems += 1;
    }
    return _return;
}

Auth * PTree::queryValue(const uint16_t & index){
    if(index > numElems){
        //cerr << "[Error] PTree::queryValue() -- illegal parameter. " << endl;
        LOGERROR("[Error] PTree::queryValue() -- illegal parameter.");
        return NULL;
    }

    uint16_t offset = index;
    uint16_t numOfCorner = this->depth - 2;

    bool LeftOrRight[numOfCorner];
    for(uint16_t i = 0; i < numOfCorner; i++){
        offset /= 2;
        if(offset % 2 == 1){
            LeftOrRight[numOfCorner - i - 1] = true;
        }else{
            LeftOrRight[numOfCorner - i - 1] = false;
        }
    }

    CtxtNode * pointOfValues = this->rootValues;
    for(uint16_t i = 0; i < numOfCorner; i++){
        if(LeftOrRight[i]){
            pointOfValues = pointOfValues->getRightChild();
        }else{
            pointOfValues = pointOfValues->getLeftChild();
        }
    }

    //bool _res = true;
    //this->db->startSQL();
    //string strWeights[numOfCorner];
    CtxtSiblingPathNode *cspNode = new CtxtSiblingPathNode(NULL);
    CtxtSiblingPathNode *pp = cspNode;
    for(uint16_t i = 0; i < numOfCorner; i++){
        CtxtNode * brother = NULL;
        if(LeftOrRight[numOfCorner - i - 1]){
            brother = pointOfValues->getParent()->getLeftChild();
        }else{
            brother = pointOfValues->getParent()->getRightChild();
        }
        CtxtSiblingPathNode *cspnodetmp = new CtxtSiblingPathNode(this->cy->getPubKey());
        cspnodetmp->setWeight(brother->getWeight());
        pp->setNext(cspnodetmp);
        pp = cspnodetmp;
        /*
        strWeights[i] = this->db->queryDB("values_p", brother->getID());
        if(strWeights[i].empty()){
            //cerr << "[Error] PTree::queryValue() -- Getting value from DB is NOT OK" << endl;
            LOGERROR("[Error] PTree::queryValue() -- Getting value from DB is NOT OK!");
            _res = false;
            break;
        }*/
        pointOfValues = pointOfValues->getParent();
    }
    /*string strQueryValue = this->db->queryDB("leaves", index);
    string strBrotherValue = this->db->queryDB("leaves", (index % 2==0) ? (index + 1) : (index - 1));
    if(strQueryValue.empty() || strBrotherValue.empty()){
        //cerr << "[Error] PTree::queryValue() -- Getting value from DB is NOT OK" << endl;
        LOGERROR("[Error] PTree::queryValue() -- Getting value from DB is NOT OK!");
        _res = false;
    }
    this->db->endSQL(_res);
    if(_res){
        DSAuth ds;
        ds.putSomething("num", to_string(numOfCorner));
        ds.putSomething("query-value", strQueryValue);
        ds.putSomething("brother-value", strBrotherValue);
        for(uint16_t i = 0; i < numOfCorner; i++){
            ds.putSomething("sibling-path-" + to_string(i), strWeights[i]);
        }
        return ds.toString();
    }*/
    Auth *auth = new Auth();
    auth->setNum(numOfCorner);
    auth->setQueryValue(this->leaves[index]);
    auth->setBrotherValue(this->leaves[(index % 2 == 0) ? (index + 1) : (index - 1)]);
    auth->setSiblingPath(cspNode);

    return auth;
}

void PTree::printPTree(){
    //cout << "[Info] the weights tree structure is as follows..." << endl;
    //printTree(this->rootWeights);
    //cout << "[Info] the values tree structure is as follows..." << endl;
    //printTree(this->rootValues);
    vector<ZZ>::iterator it;
    for(it = leaves.begin(); it != leaves.end();it++){
        cout << *it << endl;
    }
}

CtxtNode * PTree::getPosition(CtxtNode * root){
    queue<CtxtNode *> q;
    q.push(root);
    do{
        CtxtNode * node = q.front();
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
void PTree::printTree(ZZNode * root){
    vector<ZZNode *> vec;
    vec.push_back(root);
    uint16_t cur = 0;
    uint16_t last = 0;
    while(cur < vec.size()){
        last = vec.size();
        while(cur < last){
            cout << vec[cur]->getWeight() << " ";
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
    vector<ZZNode *>().swap(vec);
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
}*/

bool PTree::isFull(){
    if(getNumElems() == getMaxElems()) return true;
    return false;
}

uint16_t PTree::getMaxElems(){
    return this->maxElems;
}

uint16_t PTree::getNumElems(){
    return this->numElems;
}

uint16_t PTree::getDepth(){
    return this->depth;
}

void PTree::deleteTree(CtxtNode * root){
    if(root == NULL){
        return;
    }else{
        deleteTree(root->getLeftChild());
        deleteTree(root->getRightChild());
        delete root;
    }
}

string PTree::ZZ2Bytes(const ZZ & x){
    /*unsigned char pstr[sizeof(ZZ)]; // size of ZZ is 8
    BytesFromZZ(pstr, x, sizeof(ZZ));
    string _return = base64_encode(pstr, sizeof(ZZ));
    return _return;*/
    return "";
}

ZZ PTree::Bytes2ZZ(const string & x){
    /*string y = base64_decode(x);
    ZZ _return = ZZFromBytes((const unsigned char *)(y.c_str()), sizeof(ZZ));
    return _return;*/
    return to_ZZ(0);
}

/*
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
}*/



string PTree::test(){
    //string strv = this->db->queryDB("values_p", this->rootValues->getID());
    //Ctxt * cv = this->cy->Bytes2Ctxt(strv);
    //ZZX * pv = this->cy->decrypt(*cv);
    //cout << *pv << endl;
    return "";
}
