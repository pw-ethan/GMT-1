#include <iostream>
#include <time.h>

#define DEBUG_DB

#ifdef DEBUG_DB
#include "DBUtility.h"
#include "config.h"
#include "common.h"
#include "VTree.h"
#endif


#ifdef DEBUG_VTREE
#include <NTL/ZZ.h>
#include "VTree.h"
#include "common.h"

using namespace NTL;
#endif

using namespace std;

int main()
{
    clock_t startTime, endTime;
    startTime = clock();
//===============================================

#ifdef DEBUG_VTREE
    VTree * vt = new VTree();
    vt->printVTree();

    uint16_t ids1[] = {1};
    uint16_t ids2[] = {1, 2};
    uint16_t ids4[] = {1, 2, 3, 4};

    vt->updateVTree(ids1, 1);
    vt->printVTree();
    vt->updateVTree(ids2, 2);
    vt->printVTree();
    vt->updateVTree(ids4, 4);
    vt->printVTree();

    cout << "Evidence: " << vt->getEvidence() << endl;

    ZZ tmp = to_ZZ("234");
    cout << tmp << endl;
    cout << "size of ZZ: " << sizeof(tmp) << endl;
    string stra = toBytes(&tmp, sizeof(tmp));
    
    string strb = stra;

    ZZ tmpt = vt->Bytes2ZZ(strb);

    cout << tmpt << endl;

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
    DBUtility *db = new DBUtility();
    db->initDB(HOST, USER, PWD, DB_NAME);

    db->startSQL();
    bool ret = true;
    /*
    ZZ tmp = to_ZZ("987654321");
    string stmp = toBytes(&tmp, sizeof(tmp));
    for(int i = 5; i < 10; i++){
        if(!(db->updateDB("weights", i, stmp))){
            ret = false;
            break;
        }
    }
    */
    string result = db->queryDB("weights", 1);
    cout << result << endl;
    db->endSQL(ret);

    VTree * vt = new VTree();
    cout << "tag 1" << endl;
/////here is a problem
    ZZ tmp = vt->Bytes2ZZ(result);
    
    cout << "tag 2" << endl;

    cout << tmp << endl;

    delete vt;

    delete db;
#endif

//===============================================
    endTime = clock();
    cout << "\nThe running time of this program is " << (double)(endTime - startTime) / CLOCKS_PER_SEC << " secs." << endl;
    return 0;
}


