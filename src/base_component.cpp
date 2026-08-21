#include <base_component.h>

Base_Component::Base_Component() {}

Base_Component::~Base_Component() {}

std::string Base_Component::get_name() {
    return this->name;
}

bool Base_Component::is_setup() {
    return this->setup;
}

bool Base_Component::is_active() {
    return this->active;
}

bool Base_Component::is_removed() {
    return this->removed;
}

void Base_Component::set_name(std::string name) {
    this->name = name;
}

void Base_Component::set_active(bool active) {
    this->active = active;
}

void Base_Component::set_removed(bool removed) {
    this->removed = removed;
}

void Base_Component::attach_from_node(Base_Node* target, void* global_data) {
    this->attach(target, global_data);
    this->setup = true;
}

void Base_Component::apply_from_node(Base_Node* target, float delta_time, void* global_data) {
    this->update_information(target, delta_time, global_data);
}

void Base_Component::detach_from_node(Base_Node* target, void* global_data) {
    this->setup = false;
    this->detach(target, global_data);
}

void Base_Component::attach(Base_Node* target, void* global_data) {}

void Base_Component::detach(Base_Node* target, void* global_data) {}

void Base_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {}

void Base_Component::draw(Base_Node* target, int& draw_index) {}

void Base_Component::update_information(Base_Node* target, float delta_time, void* global_data) {}