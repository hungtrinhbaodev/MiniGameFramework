#include <node.h>

#include <algorithm>
#include <iostream>

Node::Node() {}

Node::~Node() {
    for (Base_Action* action : actions) {
        delete (action);
    }
}

void Node::do_action(Base_Action* action, int tag) {
    if (action == nullptr) {
        return;
    }
    action->set_tag(tag);
    action->with_start_state(transform, this);
    action->set_target(this);
    actions.push_back(action);
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

void Node::attach() {}

void Node::detach() {}

void Node::draw(Transform& world_transform, int& draw_index) {
    // Draw nothing because I am a node
}

void Node::enter() {
    this->detach();
}

void Node::exit() {
    this->detach();
    this->stop_all_action();
}

void Node::update(float delta_time) {
    Base_Node::update(delta_time);
    cleanup_stopped_actions();

    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        bool is_finish_all = action->travel(transform, delta_time);
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