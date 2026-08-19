#include <base_action.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <utils.h>

#include <algorithm>
#include <iostream>

Base_Action::Base_Action() {}

Base_Action::~Base_Action() {
    // clearn all chain sequence and spawn first
    deep_clean();
}

void Base_Action::set_global_data(void* global_data) {
    this->global_data = global_data;
}

void Base_Action::set_debug(bool debug) {
    this->debug = debug;
}

void Base_Action::set_removed(bool removed) {
    this->removed = removed;
}

void Base_Action::assign_target_to_all_chain(Base_Node* target, void* global_data, bool debug) {
    this->setup_target_to_action(target);
    if (this->global_data == nullptr) {
        this->global_data = global_data;
    }
    if (debug) {
        this->start_chain_time = Utils::now();
    }
    this->is_setup = true;
    if (this->next_spawn_chain != nullptr) {
        this->next_spawn_chain->assign_target_to_all_chain(target, global_data, debug);
    }
}

void Base_Action::assign_target_2(Base_Node* target, void* global_data, bool debug) {
    this->setup_target_to_action(target);
    if (this->global_data == nullptr) {
        this->global_data = global_data;
    }
    if (debug) {
        this->start_chain_time = Utils::now();
    }
    this->is_setup = true;
}

bool Base_Action::travel_action(Base_Node* target, float delta_time, void* global_data) {
    return travel(target, delta_time, global_data, this->processing_informations, -1, this->debug);
}

bool Base_Action::travel_action_2(Base_Node* target, float delta_time, void* global_data) {
    long start_action_time = -1;
    bool finish_all =
        this->update_action(target, delta_time, global_data, processing_informations, start_action_time, debug);
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

void Base_Action::setup_target_to_action(Base_Node* target) {}

bool Base_Action::update_action(
    Base_Node* target,
    float delta_time,
    void* global_data,
    std::vector<Action_Processing_Information>& processing_informations,
    long& start_time_chain,
    bool debug
) {
    if (!this->is_setup) {
        this->assign_target_2(target, global_data, debug);
    }
    if (start_time_chain < 0) {
        start_time_chain = this->start_chain_time;
    }
    bool finish_action = true;
    if ((!this->is_end() || this->type == Action_Type::ALWAY_HAPPEN) && this->is_valid_target(target)) {
        if (this->type == Action_Type::ALWAY_HAPPEN) {
            if (!this->is_appled) {
                this->apply(target, delta_time);
                this->is_appled = true;
            }
        } else {
            this->apply(target, delta_time);
            finish_action = false;
        }
    }
    if (debug && finish_action && !this->added_into_processing_informations) {
        long current_time = Utils::now();
        processing_informations.push_back(
            {(float)(this->start_chain_time - start_time_chain) / 1000,
             (float)(current_time - start_time_chain) / 1000,
             this->get_action_name()}
        );
        this->added_into_processing_informations = true;
    }
    return finish_action;
}

bool Base_Action::spawn(
    Base_Node* target,
    float delta_time,
    void* global_data,
    std::vector<Action_Processing_Information>& processing_informations,
    long& start_time_chain,
    bool debug
) {
    bool finish_action =
        this->update_action(target, delta_time, global_data, processing_informations, start_time_chain, debug);
    if (this->next_spawn_chain != nullptr) {
        bool finish_next_spawn = this->next_spawn_chain->spawn(
            target, delta_time, global_data, processing_informations, start_time_chain, debug
        );
        return finish_action && finish_next_spawn;
    }
    return finish_action;
}

bool Base_Action::sequence(
    Base_Node* target,
    float delta_time,
    void* global_data,
    std::vector<Action_Processing_Information>& processing_informations,
    long& start_time_chain,
    bool debug
) {
    bool finish_action =
        this->update_action(target, delta_time, global_data, processing_informations, start_time_chain, debug);
    if (finish_action && this->next_sequence_chain != nullptr) {
        finish_action &= this->next_sequence_chain->sequence(
            target, delta_time, global_data, processing_informations, start_time_chain, debug
        );
    }
    return finish_action;
}

bool Base_Action::travel(
    Base_Node* target,
    float delta_time,
    void* global_data,
    std::vector<Action_Processing_Information>& processing_informations,
    long start_time_chain,
    bool debug
) {
    if (!this->is_setup) {
        this->assign_target_to_all_chain(target, global_data, debug);
    }
    if (debug && start_time_chain < 0) {
        start_time_chain = this->start_chain_time;
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
    if (debug && finish_all && !this->added_into_processing_informations) {
        long current_time = Utils::now();
        processing_informations.push_back(
            {(float)(this->start_chain_time - start_time_chain) / 1000,
             (float)(current_time - start_time_chain) / 1000,
             this->get_action_name()}
        );
        this->added_into_processing_informations = true;
    }
    if (this->next_spawn_chain != nullptr) {
        finish_all &= this->next_spawn_chain->travel(
            target, delta_time, global_data, processing_informations, start_time_chain, debug
        );
    }
    if (finish_all) {
        if (this->next_sequence_chain != nullptr) {
            if (!this->next_sequence_chain->is_setup) {
                this->next_sequence_chain->assign_target_to_all_chain(target, global_data, debug);
            }
            finish_all &= this->next_sequence_chain->travel(
                target, delta_time, global_data, processing_informations, start_time_chain, debug
            );
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

std::string Base_Action::get_action_name() {
    return "Base Action";
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

bool Base_Action::is_removed() {
    return this->removed;
}

bool Base_Action::is_debug() {
    return this->debug;
}

void Base_Action::show_debug() {
    std::sort(processing_informations.begin(), processing_informations.end());
    for (auto& debug : this->processing_informations) {
        std::cout << debug;
    }
    std::cout << std::endl;
}

bool Base_Action::is_end() {
    return current_duration >= total_duration;
}

int Base_Action::get_tag() {
    return tag;
}

float Base_Action::get_rate() {
    float rate = current_duration / total_duration;
    switch (ease_type) {
        case Action_Ease::SINE_OUT: {
            rate = sin(rate * M_PI / 2);
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
    rate = std::max(0.f, std::min(1.f, rate));
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
    processing_informations.clear();
    this->added_into_processing_informations = false;
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