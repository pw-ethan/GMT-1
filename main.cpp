#include <iostream>
#include <time.h>
#include <NTL/ZZ.h>

#include "common.h"
#include "memory_dump.h"
#include "config.h"
#include "CryptoUtility.h"

//#define DEBUG_INIT
//#define DEBUG_CRYPTO
#define DEBUG_C
#include "CtxtNode.h"
#include "ZZNode.h"

#ifdef DEBUG_JSON
#include "DSAuth.h"
#endif

#ifdef DEBUG_C
#include "VTree.h"
#include "PTree.h"
#include "Auth.h"
#include "CtxtSiblingPathNode.h"
//#include "DSAuth.h"
#endif

#ifdef DEBUG_CRYPTO
#include "CryptoUtility.h"
#include "DBUtility.h"
#include "VTree.h"
#endif

#ifdef DEBUG_PTREE
#include "PTree.h"
#endif

#ifdef DEBUG_DB
#include "DBUtility.h"
#include "VTree.h"
#include "Base64.h"
#endif


#ifdef DEBUG_VTREE
#include "VTree.h"
#endif

using namespace std;
using namespace NTL;


int main()
{
    clock_t startTime, endTime;
    startTime = clock();
//===============================================

#ifdef DEBUG_C

    srand((unsigned)(time(NULL)));

    // Initialization - Verifier's Tree
    VTree *vt = new VTree();

    // Initialization - Prover's Tree
    PTree *pt = new PTree();
    clock_t time_point_one, time_point_two;
    int time_takes = 0;
    int time_native = 0;

    uint16_t numA = 4;

    //cin >> numA;

    // 16 times insertion and some times random query
    for(uint16_t i = 0; i < numA; i++){
        // Check whether it is full, if so, update
        if(vt->isFull()){
            // Generate weights(ZZ type)
            uint16_t numAdd2Weights = vt->getNumAdd2Weights();
            ZZ * weights = vt->genWeights(numAdd2Weights);

            // Update Verifier
            vt->updateVTree(weights, numAdd2Weights);
            //vt->printVTree();

            // Encrypt weights and publish
            //string strWeights[numAdd2Weights];
            //vt->weights2Str(weights, strWeights, numAdd2Weights);
            CtxtSiblingPathNode* pweights = vt->genCtxtWeights(weights, numAdd2Weights);
            cout << *(vt->cy->decrypt(*(pweights->getNext()->getWeight())));
            // Update Prover
            pt->updatePTree(pweights, numAdd2Weights);
            if(i > 1){
                cout << endl;
                cout << "data num : " << vt->getNumElems() << endl;
                double sumtime = (double) time_takes / CLOCKS_PER_SEC;
                cout << "Total time : " << sumtime << " secs." << endl;
                cout << "Average time : " << sumtime / vt->getNumElems() << " secs." << endl;
                cout << "Native time : " << (double) time_native / CLOCKS_PER_SEC / numA << endl << endl;
            }

        }
        // Insert value to Verifier and Prover
        ZZ value = genRandomValue();
        cout << "[Info] add value-" << i << " : " << value << endl;

        time_point_one = clock();
        vt->addValue(value);
        time_point_two = clock();
        time_native += time_point_two - time_point_one;

        pt->addValue(value);
        time_point_two = clock();
        time_takes += time_point_two - time_point_one;
        // random query
        /*if(i > 2 && isItTime()){
            string result = pt->queryValue(i - 1);

            bool res = vt->verify(i - 1, result);
            cout << "[Info] verify result : " << (res==true ? "Y" : "N") << endl;
        }*/
    }



    delete pt;
    delete vt;

#endif

#ifdef DEBUG_SOCKET
/*
    VTree *vt = new VTree();
    vt->printVTree();

    cout << vt->getEvidence() << endl;

    ZZ tmp = to_ZZ("2");

    for(int i = 0; i < 4; i++){
        if(vt->getNumElems() == vt->getMaxElems()){
            uint16_t numAdd2Weights = vt->getNumAdd2Weights();
            ZZ * weights = vt->genWeights(numAdd2Weights);
            vt->updateVTree(weights, numAdd2Weights);
            vt->printVTree();
        }
        cout << endl << "add " << i << "th value" << endl;
        vt->addValue(tmp);
        cout << "Evidence: " << vt->getEvidence() << endl;
    }

    delete vt;
*/    
    CryptoUtility * cy = new CryptoUtility();
    cy->initFHEByVerifier();

    ZZ tmp = to_ZZ("123");
    Ctxt *ctmp = cy->encrypt(tmp);

    ZZNode *znode = new ZZNode(tmp);
    CtxtNode *node = new CtxtNode(cy->getPubKey(), ctmp);

    ZZX *ptmp = cy->decrypt(*(node->getWeight()));
    cout << *ptmp << endl;
    cout << znode->getWeight() << endl;

    delete znode;
    delete node;

#endif



#ifdef DEBUG_PTREE

    PTree * pt = new PTree();
    pt->printPTree();
    
    CryptoUtility *cy = new CryptoUtility();
    cy->initFHEByVerifier();

    ZZ tmp = to_ZZ("2");

    for(int i = 0; i < 4; i++){
        if(pt->getNumElems() == pt->getMaxElems()){
            uint16_t numAdd2Weights = power_two(pt->getDepth());
            ZZ * weights = gen_weights(numAdd2Weights);
            string strWeights[numAdd2Weights];
            for(uint16_t i = 0; i < numAdd2Weights; i++){
                strWeights[i] = cy->Ctxt2Bytes(*(cy->encrypt(weights[i])));
            }
            pt->updatePTree(strWeights, numAdd2Weights);
            pt->printPTree();
        }
        cout << endl << "add " << i << "th value" << endl;
        pt->addValue(tmp);
    }

    string strv = pt->test();
    Ctxt * cv = cy->Bytes2Ctxt(strv);
    ZZX * pv = cy->decrypt(*cv);
    cout << *pv << endl;

    delete cy;
    delete pt;

#endif


#ifdef DEBUG_INIT
    CryptoUtility *cy = new CryptoUtility();
    cy->initFHE();


    delete cy;
#endif

#ifdef DEBUG_JSON

    DSAuth ds1;
    string k1 = "k1";
    string v1 = "v1";
    string k2 = "k2";
    string v2 = "v2";

    ds1.putSomething(k1, v1);
    ds1.putSomething(k2, v2);

    string json = ds1.toString();

    LOGI("%s", "hello");

    DSAuth ds2;
    ds2.fromString(json);
    
    cout << ds2.getSomething(k1) << endl;
    cout << ds2.getSomething(k2) << endl;


#endif



#ifdef DEBUG_CRYPTO

    VTree *vt = new VTree();

    CryptoUtility * cy = new CryptoUtility();
    cy->initFHE();

    clock_t timePointA, timePointB;
    //timePointA = clock();

    int timeTakes = 0;

    ZZ tmp = to_ZZ("123");
    Ctxt * ctmp = cy->encrypt(tmp);

    ZZ * datas = vt->genWeights(32);
    for(int i = 0; i < 7; i++){
        //cout << datas[i] << endl;
        Ctxt * ca = cy->encrypt(datas[i]);
        timePointA = clock();
        *ctmp *= *ca;
        timePointB = clock();
        timeTakes += timePointB - timePointA;
        //ZZX * pa = cy->decrypt(*ca);
        //cout << *pa << endl;
    }
    cout << "+ time : " << (double) timeTakes / CLOCKS_PER_SEC / 7 << " secs." << endl;
    //timePointB = clock();
    //cout << "en & de takes - " << (double)(timePointB - timePointA) / CLOCKS_PER_SEC / 32 << " secs." << endl;
/*
    CryptoUtility * cyp = new CryptoUtility();
    cyp->initFHEByProver();

    ZZ a = to_ZZ("12");
    cout << "a: " << a << endl;

    Ctxt * pct = cyp->encrypt(a);
    *pct *= *pct;

    string sct = cyp->Ctxt2Bytes(*pct);
    
    cout << sct.length() << endl;

    CryptoUtility * cyv = new CryptoUtility();
    cyv->initFHEByVerifier();

    Ctxt *pct2 = cyv->Bytes2Ctxt(sct);

    ZZX * ppt = cyv->decrypt(*pct2);
    cout << *ppt << endl;

    delete cyp;
    delete cyv;
*/
    delete cy;
    delete vt;

#endif

#ifdef DEBUG_VTREE
    VTree * vt = new VTree();
    vt->printVTree();

    cout << "Evidence: " << vt->getEvidence() << endl;

    ZZ tmp = to_ZZ("2");

    for(int i = 0; i < 4; i++){
        if(vt->getNumElems() == vt->getMaxElems()){
            uint16_t numAdd2Weights = power_two(vt->getDepth());
            ZZ * weights = gen_weights(numAdd2Weights);
            vt->updateVTree(weights, numAdd2Weights);
            vt->printVTree();
        }
        cout << endl << "add " << i << "th value" << endl;
        vt->addValue(tmp);
        cout << "Evidence: " << vt->getEvidence() << endl;
    }

    delete vt;
#endif
    /*
    // 初始化全同态加密算法，生成公私钥对
    crypto_fhe_utility *cy = new crypto_fhe_utility();
    FHEPubKey *publicKey = cy->getPublicKey();
    FHESecKey *secretKey = cy->getSecretKey();


    // 生成权重值 weights
    int num_weights = power_two(TREE_LEVELS) - 1;
    ZZ * weights = gen_weights(num_weights);

    // 使用全同态加密算法加密权重 weights，并将加密后的结果给 verifier
    void *tmp_en_weights = operator new[](num_weights * sizeof(Ctxt));
    Ctxt *en_weights = static_cast<Ctxt *>(tmp_en_weights);
    for(int i = 0; i < num_weights; i++){
        new (&en_weights[i])Ctxt(*publicKey);
    }
    for(int i = 0; i < num_weights; i++){
        cout << "encrypt weights[" << i << "] : " << weights[i] << endl;
        publicKey->Encrypt(en_weights[i], to_ZZX(weights[i]));
    }
    cout << endl;

    // 初始化 verifier
    v_tree* vt = new v_tree(secretKey, weights);

    // 初始化 prover
    p_tree* pt = new p_tree(publicKey, en_weights);

    delete[] weights;
    for(int i = 0; i < num_weights; i++){
        en_weights[i].~Ctxt();
    }
    operator delete[] (en_weights);
    
    // 随机生成数据，将数据发送给 verifier
    int num = power_two(TREE_LEVELS - 1);//leaves of the tree

    srand((unsigned)time(NULL));
    for(int i = 0; i<num;i++){
        int value = rand()%WEIGHT_BOUNDARY + 1;
        cout << "append "<< i  << "th value " << value << " to verifier & prover tree." << endl;
        vt->vt_add(to_ZZ(value));
        cout << endl;

        Ctxt ct_value(*publicKey);
        publicKey->Encrypt(ct_value, to_ZZX(value));
        pt->pt_add(ct_value);
        cout << endl;
    }
    cout << "verifier tree :" << endl;
    vt->vt_print();


    int index = 0;
    int choice = 1;
    while (choice) {
        cout << "\ninput the index of query data:";
        cin >> index;

        // 查询数据
        auth_ds *auth = pt->pt_query(index);

        ZZX query_data;
        secretKey->Decrypt(query_data, auth->sibling_paths[TREE_LEVELS - 1]);
        cout << "\nquery data is " << query_data[0] << endl;

        cout << "\nsilibing path : " << endl;
        for(int i = 0; i < TREE_LEVELS - 1; i++){
            ZZX sp;
            secretKey->Decrypt(sp, auth->sibling_paths[i]);
            cout << "level[" << TREE_LEVELS - 1 - i << "] : " << sp[0] << endl;
        }


        // verify
        int result = vt->vt_verify(index, auth);

        cout << "\nverify result : " << (result == 1 ? "SUCCEED!" : "FAILED!") << endl;

        cout << "\ncontinue(1) or break(0):" << endl;
        cin >> choice;
    }

    
    // 释放内存
    publicKey = NULL;
    secretKey = NULL;
    delete publicKey;
    delete secretKey;
    
    delete cy;

*/
#ifdef DEBUG_DB

    VTree * vt = new VTree();

    DBUtility *db = new DBUtility();
    db->initDB(HOST, USER, PWD, DB_NAME);

    db->startSQL();
    bool ret = true;
/*
    ZZ tmpa = to_ZZ("123456");
    string stmp = vt->ZZ2Bytes(tmpa);

    for(int i = 0; i < 5; i++){
        if(!(db->updateDB("weights", i, stmp))){
            ret = false;
            break;
        }
    }
*/
    string result = db->queryDB("weights", 1);

    db->endSQL(ret);

    ZZ tmpb = vt->Bytes2ZZ(result);
    cout << tmpb << endl;

    delete vt;

    delete db;
#endif

//===============================================
    endTime = clock();
    cout << "\nThe running time of this program is " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " secs." << endl;
    return 0;
}


