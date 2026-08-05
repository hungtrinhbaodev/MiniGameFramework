#include <node.h>

#include <algorithm>

Node::Node() {
}

Node::~Node() {
}

void Node::do_action(Base_Action* action, int tag) {
    if (action == nullptr) {
        return;
    }
    action->set_tag(tag);
    action->with_start_state(transform);
    actions.push_back(action);
}

void Node::stop_action(int tag) {
    Base_Action* action = nullptr;
    for (Base_Action*& current_action : actions) {
        if (current_action->get_tag() == tag) {
            action = current_action;
            current_action = actions[actions.size() - 1];
            actions.pop_back();
            break;
        }
    }
    if (action != nullptr) {
        delete (action);
    }
}

void Node::draw(Transform& world_transform) {
    // Draw nothing because I am a node
}

void Node::update(float delta_time) {
    for (int i = 0; i < actions.size(); i++) {
        Base_Action* action = actions[i];
        bool is_finish_all = action->travel(transform, delta_time);
        if (is_finish_all) {
            delete (action);
            actions[i] = actions[actions.size() - 1];
            actions.pop_back();
            i--;
        }
    }
}