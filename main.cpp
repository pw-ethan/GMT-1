#include <iostream>
#include <time.h>
#include <NTL/ZZ.h>

#include "common.h"
#include "memory_dump.h"

#define DEBUG_CRYPTO

#ifdef DEBUG_CRYPTO
#include "CryptoUtility.h"
#endif

#ifdef DEBUG_PTREE
#include "PTree.h"
#endif

#ifdef DEBUG_DB
#include "DBUtility.h"
#include "config.h"
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

#ifdef DEBUG_CRYPTO

    //CryptoUtility * cy = new CryptoUtility();
    //cy->initFHE();

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
    //delete cy;

#endif


#ifdef DEBUG_PTREE

    CryptoUtility *cy = new CryptoUtility();
    cy->initFHE();

    PTree * pt = new PTree();
    pt->printPTree();

    ZZ v2 = to_ZZ("2");
    ZZ v3 = to_ZZ("3");

    Ctxt * ct2 = new Ctxt(*(cy->getPubKey()));
    Ctxt * ct3 = new Ctxt(*(cy->getPubKey()));

    cy->getPubKey()->Encrypt(*ct2, to_ZZX(v2));
    cy->getPubKey()->Encrypt(*ct3, to_ZZX(v3));

    string str2 = pt->Ctxt2Bytes(ct2);
    
    Ctxt *ctt2= new Ctxt(*(cy->getPubKey()));
    ctt2 = pt->Bytes2Ctxt(str2);

    memory_dump(ct2, sizeof(Ctxt));
    memory_dump(ctt2, sizeof(Ctxt));

    Ctxt ctSum(*ctt2);
    ctSum += *ct3;

    ZZX vSum;
    cy->getSecKey()->Decrypt(vSum, ctSum);

    cout << "2 + 3 = " << vSum[0] << endl;

/*
    for(uint16_t i = 0; i < 3; i++){
        uint16_t numAdd2Weights = power_two(i);
        uint16_t ids[numAdd2Weights];
        for(uint16_t j = 0; j < numAdd2Weights; j++){
            ids[j] = j + 1;
        }
        pt->updatePTree(ids, numAdd2Weights);
        pt->printPTree();
    }
*/
    //delete ctt2;
    delete ct3;
    delete ct2;
    delete pt;
    delete cy;

#endif

#ifdef DEBUG_VTREE
    VTree * vt = new VTree();
    vt->printVTree();
/*
    for(uint16_t i = 0; i < 3; i++){
        uint16_t numAdd2Weights = power_two(i);
        ZZ * weights = gen_weights(numAdd2Weights);
        vt->updateVTree(weights, numAdd2Weights);
        vt->printVTree();
    }
*/
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


