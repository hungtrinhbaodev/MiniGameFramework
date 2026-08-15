#include <base_action.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

Base_Action::Base_Action() {}

Base_Action::~Base_Action() {
    // clearn all chain sequence and spawn first
    deep_clean();
}

void Base_Action::set_target(Base_Node* target) {
    this->target = target;
}

void Base_Action::set_global_data(void* global_data) {
    this->global_data = global_data;
}

void Base_Action::assign_target_to_all_chain(Base_Node* target, void* global_data) {
    this->setup_target_to_action(target);
    if (this->target == nullptr) {
        this->target = target;
    }
    if (this->global_data == nullptr) {
        this->global_data = global_data;
    }
    this->is_setup = true;
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->assign_target_to_all_chain(target, global_data);
    }
}

void Base_Action::setup_target_to_action(Base_Node* target) {}

bool Base_Action::travel(Base_Node* target, float delta_time, void* global_data) {
    if (!this->is_setup) {
        this->assign_target_to_all_chain(target, global_data);
    }
    bool finish_all = true;
    if ((!this->is_end() || this->type == Action_Type::ALWAY_HAPPEN) && this->is_valid_target(target)) {
        if (this->type == Action_Type::ALWAY_HAPPEN) {
            if (!this->is_appled) {
                this->apply(target, delta_time);
                this->is_appled = true;
            }
        } else {
            this->apply(target, delta_time);
            finish_all = false;
        }
    }
    if (this->next_spawn_chain != nullptr) {
        finish_all &= this->next_spawn_chain->travel(target, delta_time, global_data);
    }
    if (finish_all) {
        if (this->next_sequence_chain != nullptr) {
            if (!this->next_sequence_chain->is_setup) {
                this->next_sequence_chain->assign_target_to_all_chain(target, global_data);
            }
            finish_all &= this->next_sequence_chain->travel(target, delta_time, global_data);
        }
    }
    if (finish_all) {
        if (this->is_repeat_forever || this->repeat_time > 0) {
            this->recycle();
            finish_all = false;
            if (!this->is_repeat_forever) {
                this->repeat_time--;
            }
        }
    }
    return finish_all;
}

bool Base_Action::is_valid_target(Base_Node* target) {
    return true;
}

void Base_Action::deep_clean() {
    if (this->next_spawn_chain == nullptr && this->next_sequence_chain == nullptr) {
        return;
    }
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->deep_clean();
        delete (this->next_spawn_chain);
        this->next_spawn_chain = nullptr;
    }
    if (this->next_sequence_chain != nullptr) {
        this->next_sequence_chain->deep_clean();
        delete (this->next_sequence_chain);
        this->next_sequence_chain = nullptr;
    }
}

void Base_Action::apply(Base_Node* target, float delta_time) {
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

Base_Action* Base_Action::repeat(int repeat_time) {
    this->repeat_time = std::max(repeat_time, 0);
    return this;
}

Base_Action* Base_Action::repeat_forever() {
    this->is_repeat_forever = true;
    return this;
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
    switch (ease_type) {
        case Action_Ease::SINE_OUT: {
            rate = 1 + sin(rate * M_PI / 2);
            break;
        }
        case Action_Ease::SINE_IN: {
            rate = 1 + sin((1 - rate) * -M_PI / 2);
            break;
        }
        default: {
            break;
        }
    }
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

void Base_Action::recycle() {
    this->current_duration = 0.0f;
    if (this->type == Action_Type::ALWAY_HAPPEN) {
        this->is_appled = false;
    }
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->recycle();
    }
    if (this->next_sequence_chain != nullptr) {
        this->next_sequence_chain->recycle();
    }
}