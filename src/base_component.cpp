#include <base_component.h>

Base_Component::Base_Component() {}

Base_Component::~Base_Component() {}

std::string Base_Component::get_name() {
    return this->name;
}

bool Base_Component::has_target() {
    return this->target != nullptr;
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

void Base_Component::assign_target(Base_Node* target) {
    this->target = target;
}

void Base_Component::set_removed(bool removed) {
    this->removed = removed;
}

void Base_Component::enter() {}

void Base_Component::exit() {}

void Base_Component::handle_task() {}

void Base_Component::update_information() {}