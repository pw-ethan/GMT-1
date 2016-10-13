//
// Created by pw-ethan on 16-8-16.
//
#pragma once


#include <iostream>
#include <NTL/ZZ.h>

#include "config.h"
#include "common.h"

using namespace NTL;

class v_list
{
public:
    v_list(const int& depth, const ZZ* weights);
    virtual ~v_list();
    //void vl_add(const ZZ& value);
    //void vl_update(const ZZ& value, const int& offset);
    //ZZ vl_get_value(const int& offset);
    ZZ vl_get_weight(const int& offset);
    void vl_print(const int& depth);
private:
    int elems;       /* number of elements in the list */
    //ZZ *values;     /* pointer to the address of the first element in the list */
    ZZ *weights;    /* pointer to the address of the first weight in the list */
};

