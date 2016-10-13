//
// Created by pw-ethan on 16-8-16.
//
#pragma once

#include <NTL/ZZ.h>
#include <time.h>
#include "./libs/fhe/FHE.h"

using namespace NTL;
using namespace std;

/**
 *  Calculate the nth power of 2
 */
int power_two(int n);

ZZ *gen_weights(const int num);

string toBytes(void* src, int len);

//double getDouble(const string &x);

Ctxt* getCtxt(const string &x);

