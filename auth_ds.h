//
// Created by pw-ethan on 16-8-16.
//
#pragma once

#include <NTL/ZZ.h>
#include "./libs/fhe/FHE.h"

#include "config.h"


using namespace NTL;

/**
 *  Defines the Authenticated Data Structure
 */
class auth_ds
{
    public:
        auth_ds(const FHEPubKey* publicKey);
        virtual ~auth_ds();
//        Ctxt get_query_data()const;
//        Ctxt* get_sibling_paths()const;
//        void set_query_data(const Ctxt& data);
//        void set_sibling_paths(Ctxt* paths);
//    private:
//        Ctxt query_data;
        Ctxt *sibling_paths;
};
