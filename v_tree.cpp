//
// Created by pw-ethan on 16-8-16.
//
#include "v_tree.h"


/**
 *  @param[in]  weights : all weights of the tree to create
 */
v_tree::v_tree(FHESecKey* secretKey, const ZZ* weights)
{
    this->secretKey = secretKey;
    int offset = 0;
    for(int i = TREE_LEVELS - 1; i >= 0; i--){
        int num_weights = power_two(i);
        ZZ* tmpw = new ZZ[num_weights];
        for(int j = 0; j < num_weights; j++){
            tmpw[j] = weights[offset + j];
        }

        if(i == 0){
            set(tmpw[i]);
        }

        v_list *tmp = new v_list(i, tmpw);
        this->level[i] = tmp;
        delete[] tmpw;
        offset += num_weights;
    }
    this->elems = 0;
    this->root = new ZZ;
}

v_tree::~v_tree()
{
    for(int i = 0; i < TREE_LEVELS; i++){
        delete this->level[i];
    }
    delete this->root;
}

/**
 *  Adds a new element to the tree.
 *  @param[in]  value   : the value to add to the tree
 */
void v_tree::vt_add(const ZZ& value)
{
    int offset = vt_get_size();
    ZZ up_value = value;
    for(int i = TREE_LEVELS - 1; i > 0; i--){
        up_value = up_value * this->level[i]->vl_get_weight(offset);
        offset /= 2;
    }
    *(this->root) = *(this->root) + up_value;
    this->elems += 1;
}

/**
 *  query special location data
 */
/*
auth_ds* v_tree::vt_query(const int& index)
{
    int size_vt = vt_get_size();
    if(index > size_vt){
        std::cout << "[ERROR][v_tree]: illegal parameter." << std::endl;
        return NULL;
    }

    auth_ds *auth = new auth_ds();

    int offset = index;
    auth->set_query_data(this->level[TREE_LEVELS - 1]->vl_get_value(index));

    ZZ *tmpsp = new ZZ[TREE_LEVELS];
    for(int i = 0; i < TREE_LEVELS - 1; i++){
        if(offset % 2 == 0){
            tmpsp[i] = this->level[TREE_LEVELS - 1 - i]->vl_get_value(offset + 1);
        }
        else{
            tmpsp[i] = this->level[TREE_LEVELS - 1 - i]->vl_get_value(offset - 1);
        }
        offset /= 2;
    }
    tmpsp[TREE_LEVELS - 1] = this->level[0]->vl_get_value(0);
    auth->set_sibling_paths(tmpsp);
    return auth;

}
*/

int v_tree::vt_verify(const int& index, const auth_ds* auth)
{
    int offset = index;
    ZZX master;
    this->secretKey->Decrypt(master, auth->sibling_paths[TREE_LEVELS - 1]);
    Ctxt *sibling_paths = auth->sibling_paths;
    ZZX slave;
    ZZX master_weight;
    ZZX slave_weight;

    for(int i = 0; i < TREE_LEVELS - 1; i++){
//        std::cout << "master[" << i << "] : " << master[0] << std::endl;
        this->secretKey->Decrypt(slave, sibling_paths[i]);
        master_weight = this->level[TREE_LEVELS - 1 - i]->vl_get_weight(offset);
        if(offset % 2 == 0){
            slave_weight = this->level[TREE_LEVELS - 1 - i]->vl_get_weight(offset + 1);
        }
        else{
            slave_weight = this->level[TREE_LEVELS - 1 - i]->vl_get_weight(offset - 1);
        }
        master = master * master_weight + slave * slave_weight;
        offset /= 2;
    }

    //    if((master[0]) != (this->level[0]->vl_get_value(0))){
    if((master[0] % 1013) != (*(this->root) % 1013)){
        return 0;
    }
    return 1;
}


/**
 *  returns the size of the lowest level of the Algebra Hash Tree;
 *  in other terms the number of blocks in the protected storage backend
 *  @return     the number of blocks protected by the Algebra Hash tree
 */
int v_tree::vt_get_size()
{
    return this->elems;
}

/**
 *  Print a human readable representation of the Algebra Hash Tree
 */
void v_tree::vt_print()
{
    for(int i = 0; i < TREE_LEVELS; i++){
        this->level[i]->vl_print(i);
    }
}
