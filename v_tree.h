//
// Created by pw-ethan on 16-8-16.
//
#pragma once

#include "v_list.h"
#include "config.h"
#include "auth_ds.h"
#include "./libs/fhe/FHE.h"


class v_tree
{
public:
    v_tree(FHESecKey* secretKey, const ZZ* weights);
    virtual ~v_tree();
    void vt_add(const ZZ& value);
    int vt_get_size();
    void vt_print();
//    auth_ds* vt_query(const int& index);
    int vt_verify(const int& index, const auth_ds* auth);
private:
    ZZ* root;
    int elems;
    v_list* level[TREE_LEVELS];
    FHESecKey* secretKey;
};
