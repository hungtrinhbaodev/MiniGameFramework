#pragma once;
#include <base_component.h>

class Collision_Component : public Base_Component {
public:
    void get_tag();
    void get_owner_data();

    void set_tag();
    void set_owner_data();

    void enter() override;
    void exit() override;
    void update_information() override;
    void handle_task() override;

private:
    /** Using this tag to cast inner data right away!*/
    int tag;
    /** using this data to cast when collision happen */
    void* owner_data;
}