#include <base_action.h>

Base_Action::Base_Action() {
}

Base_Action::~Base_Action() {
}

void Base_Action::with_start_state(Base_Node::Transform& tranform) {
    this->setup_start_state(tranform);
    this->is_setup = true;
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->with_start_state(tranform);
    }
}

void Base_Action::setup_start_state(Base_Node::Transform& transform) {
}

bool Base_Action::travel(Base_Node::Transform& transform, float delta_time) {
    bool finish_all = true;
    if (!this->is_end()) {
        this->apply(transform, delta_time);
        finish_all = false;
    }
    if (this->next_spawn_chain != nullptr) {
        finish_all &= this->next_spawn_chain->travel(transform, delta_time);
    }
    if (finish_all) {
        if (this->next_sequence_chain != nullptr) {
            if (!this->next_sequence_chain->is_setup) {
                this->next_sequence_chain->with_start_state(transform);
            }
            finish_all &= this->next_sequence_chain->travel(transform, delta_time);
        }
    }
    return finish_all;
}

void Base_Action::apply(Base_Node::Transform& tranform, float delta_time) {
    current_duration += delta_time;
    if (current_duration > total_duration) {
        current_duration = total_duration;
    }
}

void Base_Action::push_sequence(Base_Action* action) {
    if (this->next_sequence_chain != nullptr) {
        this->next_sequence_chain->push_sequence(action);
    } else {
        this->next_sequence_chain = action;
        // continuous_action
    }
}

void Base_Action::push_spawn(Base_Action* action) {
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->push_spawn(action);
    } else {
        this->next_spawn_chain = action;
        return;  // @Hack: No more adjacent-action, so we break the recursive loop.
        // parallel_action
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