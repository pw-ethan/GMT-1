//
// Created by pw-ethan on 16-8-16.
//

#pragma once

#include "./libs/fhe/FHE.h"
#include "config.h"

class crypto_fhe_utility {
private:
    FHEcontext *context;
    FHESecKey *secretKey;
    FHEPubKey *publicKey;
public:
    crypto_fhe_utility();

    virtual ~crypto_fhe_utility();

    void init_fhe();

    FHESecKey *getSecretKey();

    FHEPubKey *getPublicKey();
};
