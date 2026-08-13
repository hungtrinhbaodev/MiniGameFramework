#pragma once
#include <base_action.h>
#include <base_component.h>
#include <base_node.h>

class Node : public Base_Node {
public:
    Node();
    ~Node();

    Base_Component* get_component_by_name(std::string name);

    void add_component(Base_Component* component);
    void do_action(Base_Action* action, int tag = -1);

    void remove_component(Base_Component* component);
    void remove_component(std::string component_name);
    void stop_action(int tag);
    void stop_all_action();

    // Override function if need to custom class from Node
    // and use something work when node enter and exit
    virtual void attach();
    virtual void detach();
    Node_Type get_type() override;

protected:
    std::vector<Base_Action*> actions;
    void handle_personal_task() override;
    void set_world_transform_information(Custom::Transform world_transform, int draw_index) override;
    void draw(Custom::Transform& world_transform, int& draw_index) override;
    void update(float delta_time) override;
    void exit() override;
    void enter() override;

private:
    std::vector<Base_Action*> cleanup_actions;
    std::vector<Base_Component*> components;
    /**
     * Note: In some case not is remove from parent but the
     * components is not remove we keep it in this vector
     * and if node enter again we add it again!
     */
    std::vector<Base_Component*> dettached_components;
    void cleanup_stopped_actions();
};