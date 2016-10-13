//
// Created by pw-ethan on 16-8-16.
//
#pragma once

#include "p_list.h"
#include "config.h"
#include "auth_ds.h"


class p_tree
{
public:
    p_tree(FHEPubKey* publicKey, const Ctxt* weights);
    virtual ~p_tree();
    void pt_add(const Ctxt& value);
    int pt_get_size();
    auth_ds* pt_query(const int& index);
private:
    int elems;
    p_list* level[TREE_LEVELS];
    FHEPubKey *publicKey;


};
