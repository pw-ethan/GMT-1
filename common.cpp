//
// Created by pw-ethan on 16-8-16.
//
#include "common.h"

/**
 *  Calculate the nth power of 2
 */
int power_two(int n)
{
    return 1<<n;
}

ZZ *gen_weights(const int num)
{
/*
    srand((unsigned)time(NULL));
    int *result = (int *)calloc(num, sizeof(int));
    if(!result){
        return NULL;
    }
    for(int i = 0; i < num; i++){
        result[i] = rand() % WEIGHT_BOUNDARY + 1;
    }
*/
    ZZ *result = new ZZ[num];
    SetSeed(to_ZZ(time(NULL)));
    for(int i = 0; i < num; i++){
        RandomLen(result[i], 16);
    }
    return result;
}

string toBytes(void* src, int len){
    string _return(len, 0);
    char *start = reinterpret_cast<char *>(src);
    for(int i = 0; i < len; i++){
        _return[i] = *(start + i);
    }
    return _return;
}
/*
double getDouble(const string &x){
    char *p = const_cast<char *>(x.c_str());
    double * _return = 0;
    _return = reinterpret_cast<double *>(p);
    return *_return;
}*/

Ctxt* getCtxt(const string &x){
    char *p = const_cast<char *>(x.c_str());
    Ctxt *_return = NULL;
    _return = reinterpret_cast<Ctxt *>(p);
    return _return;
}
