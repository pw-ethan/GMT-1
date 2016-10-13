//
// Created by pw-ethan on 16-8-16.
//
#include "p_tree.h"

p_tree::p_tree(FHEPubKey* publicKey, const Ctxt* weights)
{
    this->publicKey = publicKey;

    int offset = 0;
    for(int i = TREE_LEVELS - 1; i >= 0; i--){
        int num_weights = power_two(i);

        void *tmptmpw = operator new[](num_weights * sizeof(Ctxt));
        Ctxt *tmpw = static_cast<Ctxt *>(tmptmpw);
        for(int j = 0; j < num_weights; j++){
            new (&tmpw[j])Ctxt(*publicKey);
        }

        for(int j = 0; j < num_weights; j++){
            tmpw[j] = weights[offset + j];
        }

        p_list *tmp = new p_list(publicKey, i, tmpw);

        this->level[i] = tmp;

        for(int i = 0; i < num_weights; i++){
            tmpw[i].~Ctxt();
        }
        operator delete[] (tmpw);

        offset += num_weights;
    }

    this->elems = 0;
}

p_tree::~p_tree()
{
    for(int i = 0; i < TREE_LEVELS; i++){
        delete this->level[i];
    }
}

void p_tree::pt_add(const Ctxt& value)
{
    int offset = pt_get_size();
    Ctxt up_value = value;
    for(int i = TREE_LEVELS - 1; i >= 0; i--){
        Ctxt tmp = up_value;
        tmp += this->level[i]->pl_get_value(offset);
        this->level[i]->pl_update(tmp, offset);

        up_value *= this->level[i]->pl_get_weight(offset);

        offset /= 2;
    }
    this->elems += 1;
}

int p_tree::pt_get_size()
{
    return this->elems;
}

auth_ds* p_tree::pt_query(const int& index)
{
    int size_pt = pt_get_size();
    if(index > size_pt){
        std::cout << "[ERROR][pt_query]: illegal parameter." << std::endl;
        return NULL;
    }

    auth_ds *auth = new auth_ds(this->publicKey);

    int offset = index;

    for(int i = 0; i < TREE_LEVELS - 1; i++){
        if(offset % 2 == 0){
            auth->sibling_paths[i] = this->level[TREE_LEVELS - 1 - i]->pl_get_value(offset + 1);
        }
        else{
            auth->sibling_paths[i] = this->level[TREE_LEVELS - 1 - i]->pl_get_value(offset - 1);
        }
        offset /= 2;
    }

    auth->sibling_paths[TREE_LEVELS - 1] = this->level[TREE_LEVELS - 1]->pl_get_value(index);
    return auth;
}
