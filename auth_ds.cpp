//
// Created by pw-ethan on 16-8-16.
//
#include "auth_ds.h"

auth_ds::auth_ds(const FHEPubKey* publicKey)
{
    void *tmp = operator new[](TREE_LEVELS * sizeof(Ctxt));
    this->sibling_paths = static_cast<Ctxt *>(tmp);
    for(int i = 0; i < TREE_LEVELS; i++){
        new (&this->sibling_paths[i])Ctxt(*publicKey);
    }
}

auth_ds::~auth_ds()
{
    for(int i = 0; i < TREE_LEVELS; i++){
        this->sibling_paths[i].~Ctxt();
    }
    operator delete[] (this->sibling_paths);
}
