#pragma once
#include <base_action.h>
#include <base_node.h>

class Node : public Base_Node {
public:
    Node();
    ~Node();

    void do_action(Base_Action* action, int tag = -1);
    void stop_action(int tag);

protected:
    std::vector<Base_Action*> actions;
    void draw(Transform& world_transform) override;
    void update(float delta_time) override;
};