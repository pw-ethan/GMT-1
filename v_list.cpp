//
// Created by pw-ethan on 16-8-16.
//
#include "v_list.h"

/**
 *  @param[in]  depth   : the depth of the array list in the tree
 *  @param[in]  weights : the weight of every nodes or leaves
 */
v_list::v_list(const int& depth, const ZZ* weights)
{
    int num = power_two(depth);

    //this->values = new ZZ[num]();
    this->weights = new ZZ[num]();

    for(int i = 0; i < num; i++){
        this->weights[i] = weights[i];
    }

    this->elems = 0;
}

v_list::~v_list()
{
    //delete[] this->values;
    delete[] this->weights;
}

/**
 *  Adds a new element to the list.
 *  @param[in]  value   : the value to add to the array list
 */
/*void v_list::vl_add(const ZZ& value)
{
    this->values[this->elems] = value;
    this->elems += 1;
}*/

/**
 *  Update a specific element with the given new value
 *  @param[in]      value   : the new value
 *  @param[in]      offset  : the index/offset of the value to update
 */
/*void v_list::vl_update(const ZZ& value, const int& offset)
{
    this->values[offset] = value;
}*/

/**
 *  Return a specific element from the array list.
 *  If either the array list pointer is NULL, or the specified offset is out of bounds, the function will return NULL.
 *  @param[in]  offset  : the offset of the element to fetch
 *  @return     a requested element in the array list
 */
/*ZZ v_list::vl_get_value(const int& offset)
{
    return this->values[offset];
}*/

ZZ v_list::vl_get_weight(const int& offset)
{
    return this->weights[offset];
}

/**
 *  Print the Algebra Hash Tree array list of hashes.
 *  If the given list pointer is NULL this function will print an error message.
 *  @param[in]  depth   ： the depth of the array list in the tree
 */
void v_list::vl_print(const int& depth)
{
    int num = power_two(depth);
    std::cout << " == Algebra Hash Tree level[" << depth << "] ==\n[ ";
    for(int i = 0; i < num; i++){
        std::cout << this->weights[i] << " ";
    }
    std::cout << "]" << std::endl;
}
