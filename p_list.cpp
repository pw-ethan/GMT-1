//
// Created by pw-ethan on 16-8-16.
//
#include "p_list.h"

p_list::p_list(const FHEPubKey* publicKey, const int& depth, const Ctxt* weights)
{
    int num = power_two(depth);
    this->num_vw = num;

    void *tmptmpv = operator new[](num * sizeof(Ctxt));
    this->values = static_cast<Ctxt *>(tmptmpv);
    for(int i = 0; i < num; i++){
        new (&(this->values)[i])Ctxt(*publicKey);
    }

    void *tmptmpw = operator new[](num * sizeof(Ctxt));
    this->weights = static_cast<Ctxt *>(tmptmpw);
    for(int i = 0; i < num; i++){
        new (&(this->weights)[i])Ctxt(*publicKey);
    }

    for(int i = 0; i < num; i++){
        this->weights[i] = weights[i];
    }

//    this->elems = 0;
}

p_list::~p_list()
{
    for(int i = 0; i < this->num_vw; i++){
        this->values[i].~Ctxt();
    }
    operator delete[] (this->values);

    for(int i = 0; i < this->num_vw; i++){
        this->weights[i].~Ctxt();
    }
    operator delete[] (this->weights);
}
/*
void p_list::pl_add(const Ctxt& value)
{
    this->values[this->elems] = value;
    this->elems += 1;
}*/

void p_list::pl_update(const Ctxt& value, const int& offset)
{
    this->values[offset] = value;
}

Ctxt p_list::pl_get_value(const int& offset)
{
    return this->values[offset];
}

Ctxt p_list::pl_get_weight(const int& offset)
{
    return this->weights[offset];
}

