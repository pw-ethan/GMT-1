/*******************************************************************
 *  Copyright (C) 2016 PeanutWine Ethan - 1501680@stu.neu.edu.cn
 *  All rights reserved..
 *
 *  文件名称: Node.h
 *  简要描述:.广义Merkle Tree的节点
 *
 *  创建日期: 2016年10月13日
 *  作者: pw-ethan
 *  说明:.声明了广义Merkle Tree节点的数据结构以及方法
 *
 *  修改日期:.
 *  作者:.
 *  说明:.
 ******************************************************************/
#pragma once

#include <stdint.h>


class Node
{
public:
    Node(const uint16_t & id);
    virtual ~Node();
    
    uint16_t getID();
    void setID(const uint16_t & id);
    Node * getParent();
    void setParent(Node * node);
    Node * getLeftChild();
    void setLeftChild(Node * node);
    Node * getRightChild();
    void setRightChild(Node * node);

private:
    uint16_t id;
    Node * parent;
    Node * leftChild;
    Node * rightChild;
};

