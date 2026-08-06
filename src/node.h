#pragma once
#include <base_action.h>
#include <base_node.h>

class Node : public Base_Node {
public:
    Node();
    ~Node();

    void do_action(Base_Action* action, int tag = -1);
    void stop_action(int tag);
    void stop_all_action();

    // Override function if need to custom class from Node
    // and use something work when node enter and exit
    virtual void attach();
    virtual void detach();

protected:
    std::vector<Base_Action*> actions;
    void draw(Transform& world_transform, int& draw_index) override;
    void update(float delta_time) override;
    void exit() override;
    void enter() override;

private:
    std::vector<Base_Action*> cleanup_actions;
    void cleanup_stopped_actions();
};