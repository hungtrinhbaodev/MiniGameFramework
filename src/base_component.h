#pragma once
#include <base_node.h>

#include <string>

class Base_Component {
public:
    Base_Component();
    virtual ~Base_Component();

    std::string get_name();
    bool is_setup();
    bool is_active();
    bool is_removed();

    void set_name(std::string name);
    void set_active(bool active);
    void set_removed(bool removed);
    void attach_from_node(Base_Node* target, void* global_data);
    void apply_from_node(Base_Node* target, float delta_time, void* global_data);
    void detach_from_node(Base_Node* target, void* global_data);

    virtual void attach(Base_Node* target, void* global_data);
    /**
     * @Note: in case force delete (program exit) we don't have any global data information,
     * so when overriding this function to use please check the nullptr for global_data
     * before we use it!
     */
    virtual void detach(Base_Node* target, void* global_data = nullptr);
    /**
     * Using to debug mesh or collision when nessesary
     */
    virtual void draw(Base_Node* target, int& draw_index);
    virtual void update_information(Base_Node* target, float delta_time, void* global_data);
    virtual void handle_task(Base_Node* target, float delta_time, void* global_data);

protected:
    std::string name = "";
    bool active = true;
    bool removed = false;
    bool setup = false;
};