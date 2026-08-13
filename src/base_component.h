#pragma once
#include <base_node.h>

#include <string>

class Base_Component {
public:
    Base_Component();
    ~Base_Component();

    std::string get_name();
    bool has_target();
    bool is_active();
    bool is_removed();

    void set_name(std::string name);
    void assign_target(Base_Node* target);
    void set_active(bool active);
    void set_removed(bool removed);

    virtual void enter();
    virtual void exit();
    virtual void update_information();
    virtual void handle_task();

protected:
    std::string name = "";
    Base_Node* target = nullptr;
    bool active = true;
    bool removed = false;
};