#pragma once
#include <base_node.h>

class Node : public Base_Node {
    public:
    Node();
    ~Node();

    protected:
    void draw(Transform& world_transform) override;
    void update(float delta_time) override;
};