#include <base_action.h>

Base_Action::Base_Action() {
}

Base_Action::~Base_Action() {
}

void Base_Action::with_start_state(Base_Node::Transform& tranform) {
}

bool Base_Action::travle(Base_Node::Transform& transform, float delta_time) {
    bool finish_all = true;
    if (!is_end()) {
        apply(transform, delta_time);
        finish_all = false;
    }
    if (next_spawn_chain != nullptr) {
        finish_all &= next_spawn_chain->travle(transform, delta_time);
    }
    if (finish_all) {
        if (next_sequence_chain != nullptr) {
            finish_all &= next_sequence_chain->travle(transform, delta_time);
        }
    }
    return finish_all;
}

Base_Action* Base_Action::remove_chain_by_tag(int tag) {
    Base_Action* ret_action = this;
    if (this->tag == tag) {
        ret_action = new Base_Action();
        ret_action->next_sequence_chain = next_sequence_chain;
        ret_action->next_spawn_chain = next_spawn_chain;
    }
    if (ret_action->next_sequence_chain != nullptr) {
        Base_Action* ret = ret_action->next_sequence_chain->remove_chain_by_tag(tag);
        if (ret != ret_action->next_sequence_chain) {
            delete (ret_action->next_sequence_chain);
        }
        ret_action->next_sequence_chain = ret;
    }
    if (ret_action->next_spawn_chain != nullptr) {
        Base_Action* ret = ret_action->next_spawn_chain->remove_chain_by_tag(tag);
        if (ret != ret_action->next_spawn_chain) {
            delete (ret_action->next_spawn_chain);
        }
        ret_action->next_spawn_chain = ret;
    }
    return ret_action;
}

void Base_Action::apply(Base_Node::Transform& tranform, float delta_time) {
    current_duration += delta_time;
    if (current_duration > total_duration) {
        current_duration = total_duration;
    }
}

void Base_Action::push_sequence(Base_Action* action) {
    if (next_sequence_chain != nullptr) {
        next_sequence_chain->push_sequence(action);
    } else {
        next_sequence_chain = action;
    }
}

void Base_Action::push_spawn(Base_Action* action) {
    if (next_spawn_chain != nullptr) {
        next_spawn_chain->push_spawn(action);
    } else {
        next_spawn_chain = action;
    }
}

bool Base_Action::is_end() {
    return current_duration >= total_duration;
}

int Base_Action::get_tag() {
    return tag;
}

float Base_Action::get_rate() {
    float rate = current_duration / total_duration;
    if (rate < 0)
        rate = 0.f;
    if (rate > 1)
        rate = 1.f;
    return rate;
}

void Base_Action::set_tag(int tag) {
    this->tag = tag;
}

void Base_Action::set_type(Action_Type type) {
    this->type = type;
}

void Base_Action::set_subtype(Action_Subtype subtype) {
    this->subtype = subtype;
}

void Base_Action::set_ease_type(Action_Ease ease) {
    this->ease_type = ease;
}

void Base_Action::set_duration(float duration) {
    total_duration = duration;
}