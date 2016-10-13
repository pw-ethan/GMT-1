//
// Created by pw-ethan on 16-8-16.
//

#include "crypto_fhe_utility.h"

crypto_fhe_utility::crypto_fhe_utility() {
    init_fhe();
}

crypto_fhe_utility::~crypto_fhe_utility() {
    /*
    std::cout << "~crypto_fhe_utility()" << std::endl;
    if(context != NULL){
        delete context;
    }
    if(publicKey != NULL){
        delete publicKey;
    }
    if(secretKey != NULL){
        delete secretKey;
    }
    std::cout << "ok" << std::endl;
    */
}

void crypto_fhe_utility::init_fhe() {
    std::cout << "Initializing ..." << std::endl;
    /* initializes parameters */
    long m = 0;         // m, p, r, the native plaintext space
    long p = 1013;      // p is a prime, 模p计算
    long r = 1;
    long L = TREE_LEVELS * 2;         // the number “levels,” i.e. the number of ciphertext primes
    long c = 2;         // number of columns in key switching matrix (recommended c = 2 or c = 3)
    long w = 64;        // the Hamming weight of a secret key (w = 64 recommended)
    long d = 0;
    long k = 80;
    long s = 0;

    m = FindM(k, L, c, p, d, s, 0);

    /* initializes context */
    context = new FHEcontext(m, p, r);
    buildModChain(*context, L, c);

    /* generates keys*/
    secretKey = new FHESecKey(*context);
    publicKey = secretKey;

    secretKey->GenSecKey(w);

}

FHEPubKey *crypto_fhe_utility::getPublicKey() {
    return publicKey;
}

FHESecKey *crypto_fhe_utility::getSecretKey() {
    return secretKey;
}
