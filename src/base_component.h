#pragma once
#include <base_node.h>

class Base_Component {
public:
    Base_Component();
    ~Base_Component();

    virtual void update(Base_Node* target);
};