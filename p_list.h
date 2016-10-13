//
// Created by pw-ethan on 16-8-16.
//
#pragma once


#include "./libs/fhe/FHE.h"
#include "config.h"
#include "common.h"


class p_list
{
public:
    p_list(const FHEPubKey* publicKey, const int& depth, const Ctxt* weights);
    virtual ~p_list();
//    void pl_add(const Ctxt& value);
    void pl_update(const Ctxt& value, const int& offset);
    Ctxt pl_get_value(const int& offset);
    Ctxt pl_get_weight(const int& offset);

private:
//    int elems;
    Ctxt *values;
    Ctxt *weights;
    int num_vw;
};
