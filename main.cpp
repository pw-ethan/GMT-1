#include <iostream>
#include <NTL/ZZ.h>
#include <time.h>
#include <vector>
#include <algorithm>

#include "crypto_fhe_utility.h"
#include "v_tree.h"
#include "p_tree.h"
#include "auth_ds.h"
#include "MyDB.h"
#include "memory_dump.h"


using namespace std;
using namespace NTL;

int main()
{

    vector<int> vec;
    int icheck[] = {2, 4, 5, 8, 10, 14, 18, 32, 40, 50, 60, 72, 100, 110, 120};
    for(int i = 0; i < sizeof(icheck); i++){
        vec.push_back(icheck[i]);
    }
    vector<int>::iterator it;

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


    clock_t time_point_one, time_point_two;
    //time_point_one = clock();
    int time_takes = 0;
    int time_native = 0;

    // 随机生成数据，将数据发送给 verifier
    int num = power_two(TREE_LEVELS - 1);//leaves of the tree

    srand((unsigned)time(NULL));
    for(int i = 0; i<num;i++){
        int value = rand()%WEIGHT_BOUNDARY + 1;
        cout << "append "<< i  << "th value " << value << " to verifier & prover tree." << endl;

        time_point_one = clock();
        vt->vt_add(to_ZZ(value));
        time_point_two = clock();
        time_takes += time_point_two - time_point_one;
        time_native += time_point_two - time_point_one;

        cout << endl;

        Ctxt ct_value(*publicKey);
        publicKey->Encrypt(ct_value, to_ZZX(value));

        time_point_one = clock();
        pt->pt_add(ct_value);
        time_point_two = clock();
        time_takes += time_point_two - time_point_one;

        cout << endl;

        it = find(vec.begin(), vec.end(), i+1);
        if(it != vec.end()){
            //time_point_two = clock();
            //time_takes += time_point_two - time_point_one;
            cout << "Data num : " << i+1 << endl;
            double sumTime = (double)(time_takes) / CLOCKS_PER_SEC;
            cout << "Total time : " << sumTime << " secs." << endl;
            cout << "Average time : " << sumTime / (i+1) << " secs." << endl;
            cout << "Native time : " << (double)(time_native) / CLOCKS_PER_SEC / num * 2 << " secs." << endl;
        }
    }
    //cout << "verifier tree :" << endl;
    //vt->vt_print();

/*
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
*/

    // 释放内存
    publicKey = NULL;
    secretKey = NULL;
    delete publicKey;
    delete secretKey;
    
    delete cy;


/*
    MyDB *db = new MyDB();
    db->initDB(HOST, USER, PWD, DB_NAME);

   // db->exeSQL("insert into aht_weights values(1, '123')");
    db->startSQL();
    bool ret = true;
    for(int i = 0; i < 10; i++){
        if(!(db->insertDB("aht_weights", i, "111"))){
            ret = false;
            break;
        }
    }
    db->endSQL(ret);

    // db->exeSQL("select * from aht_weights");

    delete db;
*/

    return 0;
}


