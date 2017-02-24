/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved. 
 *
 *  文件名称: CtxtWeightsList.cpp
 *  简要描述: 
 *
 *  创建日期: 2017年02月23日
 *  作者: pw-ethan
 *  说明: 
 *
 *  修改日期: 
 *  作者: 
 *  说明: 
 ******************************************************************/

#include "CtxtWeightsList.h"

CtxtWeightsList::CtxtWeightsList(const FHEPubKey* pubkey, const int& num)
{
    void *tmp = operator new[](num * sizeof(Ctxt));
    this->cweights = static_cast<Ctxt *>(tmp);
    for(int i = 0; i < num; i++){
        new (&this->cweights[i])Ctxt(*pubkey);
    }
    this->num = num;
}

CtxtWeightsList::~CtxtWeightsList()
{
    for(int i = 0; i < this->num; i++){
        this->cweights[i].~Ctxt();
    }
    operator delete[] (this->cweights);
}
