#include <node.h>

#include <algorithm>
#include <iostream>

Node::Node() {}

Node::~Node() {
    for (Base_Action* action : actions) {
        delete (action);
    }
    for (Base_Component* component : components) {
        if (!component->is_removed()) {
            component->exit();
        }
        delete (component);
    }
    for (Base_Component* component : dettached_components) {
        delete (component);
    }
}

Base_Component* Node::get_component_by_name(std::string name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->get_name() == name) {
            return components[i];
        }
    }
    return nullptr;
}

void Node::add_component(Base_Component* component) {
    this->components.push_back(component);
}

void Node::do_action(Base_Action* action, int tag) {
    if (action == nullptr) {
        return;
    }
    action->set_tag(tag);
    action->assign_target_to_all_chain(this);
    action->set_target(this);
    actions.push_back(action);
}

void Node::remove_component(Base_Component* component) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i] == component) {
            components[i]->set_removed(true);
            components[i]->exit();
            break;
        }
    }
}

void Node::remove_component(std::string component_name) {
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->get_name() == component_name) {
            components[i]->set_removed(true);
            components[i]->exit();
        }
    }
}

void Node::stop_action(int tag) {
    Base_Action* action = nullptr;
    for (int i = 0; i < actions.size(); i++) {
        if (actions[i]->get_tag() == tag) {
            delete (actions[i]);
            actions[i] = actions[actions.size() - 1];
            actions.pop_back();
            i--;
        }
    }
}

void Node::stop_all_action() {
    for (Base_Action* action : actions) {
        cleanup_actions.push_back(action);
    }
    actions.clear();

    cleanup_stopped_actions();
}

Node_Type Node::get_type() {
    return Node_Type::NODE;
}

void Node::attach() {}

void Node::detach() {}

void Node::handle_personal_task() {
    /**We handle logic of all components of node here */
    /**First: remove the component mark removed */
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->is_removed()) {
            components[i] = components.back();
            components.pop_back();
            i--;
        }
    }
    /**Second: handle it's task logic*/
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->is_active() && components[i]->has_target()) {
            components[i]->handle_task();
        }
    }
}

void Node::set_world_transform_information(Custom::Transform world_transform, int draw_index) {
    Base_Node::set_world_transform_information(world_transform, draw_index);

    /**Assign target to component and invoke enter to start loop */
    for (int i = 0; i < components.size(); i++) {
        if (components[i]->is_removed()) {
            continue;
        }
        if (!components[i]->has_target()) {
            components[i]->assign_target(this);
            components[i]->enter();
        }
        components[i]->update_information();
    }
}

void Node::draw(Custom::Transform& world_transform, int& draw_index) {
    for (Base_Component* component : components) {
        if (component->is_active() && component->has_target()) {
            component->draw(draw_index);
        }
    }
}

void Node::enter() {
    /**Add list waitting component into list commponent again when node enter again!*/
    for (int i = 0; i < dettached_components.size(); i++) {
        dettached_components[i]->assign_target(nullptr);
        components.push_back(dettached_components[i]);
    }
    dettached_components.clear();
    this->attach();
}

void Node::exit() {
    /**Add list running component into list detach reserve wait to attach again!*/
    for (int i = 0; i < components.size(); i++) {
        components[i]->exit();
        dettached_components.push_back(components[i]);
    }
    components.clear();
    this->detach();
    this->stop_all_action();
}

void Node::update(float delta_time) {
    Base_Node::update(delta_time);
    cleanup_stopped_actions();

    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        bool is_finish_all = action->travel(this, delta_time);
        if (is_finish_all) {
            cleanup_actions.push_back(action);
            actions[i] = actions[actions.size() - 1];
            actions.pop_back();
            i--;
        }
    }
}

void Node::cleanup_stopped_actions() {
    for (Base_Action* action : cleanup_actions) {
        delete (action);
    }
    cleanup_actions.clear();
}