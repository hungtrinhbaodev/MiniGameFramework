#pragma once
#include <base_node.h>

#include <string>

class Base_Component {
public:
    Base_Component();
    ~Base_Component();

    std::string get_name();
    bool has_target();
    bool has_global_data();
    bool is_active();
    bool is_removed();

    void set_name(std::string name);
    void assign_target(Base_Node* target);
    void set_active(bool active);
    void set_removed(bool removed);
    void set_global_data(void* global_data);

    virtual void enter();
    virtual void exit();
    /**Using to debug mesh or collision when nessesary */
    virtual void draw(int& draw_index);
    virtual void update_information();
    virtual void handle_task();

protected:
    std::string name = "";
    Base_Node* target = nullptr;
    void* global_data = nullptr;
    bool active = true;
    bool removed = false;
};