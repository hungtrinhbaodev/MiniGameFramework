#include <state_machine_component.h>

float State_Machine_Component::INFITY_STATE = -1.0f;

State_Machine_Component::State_Machine_Component() {}

State_Machine_Component::~State_Machine_Component() {}

bool State_Machine_Component::is_finish_state_at(const std::string& track) {
    if (this->tracks.find(track) == this->tracks.end()) {
        return false;
    }
    return this->tracks[track].is_finish;
}

std::string State_Machine_Component::get_current_state_at(std::string track) {
    if (this->tracks.find(track) == this->tracks.end()) {
        return "";
    }
    return this->tracks[track].current_state;
}

std::string State_Machine_Component::get_last_state_processign_at(std::string track) {
    if (this->tracks.find(track) == this->tracks.end()) {
        return "";
    }
    return this->tracks[track].last_state_processing;
}

float State_Machine_Component::get_duration_state_at(std::string track) {
    if (this->tracks.find(track) == this->tracks.end()) {
        return 0.f;
    }
    return this->tracks[track].current_processing_duration;
}

float State_Machine_Component::get_max_duration_state_at(std::string track) {
    if (this->tracks.find(track) == this->tracks.end()) {
        return 0.f;
    }
    return this->tracks[track].max_state_duration;
}

float State_Machine_Component::get_duration_remain_state_at(std::string track) {
    return this->get_max_duration_state_at(track) - this->get_duration_state_at(track);
}

int State_Machine_Component::get_source_call_state_at(std::string track_name, std::string state_name) {
    if (this->tracks.find(track_name) == this->tracks.end()) {
        return -1;
    }
    Track_Information& track = this->tracks[track_name];
    if (track.states.find(state_name) == track.states.end()) {
        return -1;
    }
    return track.states[state_name].source_call_tag;
}

void State_Machine_Component::add_track(
    std::string track_name, std::function<void(Callback_Finish_State)> finish_state_callback
) {
    if (this->tracks.find(track_name) == this->tracks.end()) {
        this->tracks[track_name] = {};
        this->tracks[track_name].finish_callback = finish_state_callback;
    }
}

void State_Machine_Component::add_state_at(
    std::string track_name,
    std::string state_name,
    State_Machine_Component::Callback start_state,
    State_Machine_Component::Callback end_state
) {
    if (this->tracks.find(track_name) == this->tracks.end()) {
        std::cout << "Warning add state fail, invalid track!" << std::endl;
        return;
    }
    Track_Information& track = this->tracks[track_name];
    track.states[state_name] = {state_name, start_state, end_state};
    return;
}

void State_Machine_Component::change_state_at(
    std::string track_name, std::string state_name, float state_duration, int source_call_tag
) {
    if (this->tracks.find(track_name) == this->tracks.end())
        return;
    if (this->global_data == nullptr) {
        this->saved_states[state_name] = {track_name, state_name, state_duration, source_call_tag};
        return;
    }
    Track_Information& track = this->tracks[track_name];
    std::string current_state = track.current_state;
    if (!track.is_callback && track.states.find(current_state) != track.states.end()) {
        State_Information& state = track.states[current_state];
        /**
         * Cancel logic and action of last state before change into other state
         */
        if (state.end_state != nullptr) {
            state.end_state(this->target, this, this->global_data, state.source_call_tag);
        }
    }
    track.current_state = state_name;
    track.current_processing_duration = 0.f;
    track.max_state_duration = state_duration;
    track.is_callback = false;
    track.is_finish = false;
    if (track.states.find(state_name) != track.states.end()) {
        State_Information& state = track.states[state_name];
        state.source_call_tag = source_call_tag;
        /**
         * Cancel logic and action of last state before change into other state
         */
        if (state.start_state != nullptr) {
            state.start_state(this->target, this, this->global_data, source_call_tag);
        }
    }
}

void State_Machine_Component::log(std::string track) {
    if (this->tracks.find(track) == this->tracks.end())
        return;
    std::cout << "Track name: " << track << ", " << this->tracks[track];
}

void State_Machine_Component::attach(Base_Node* target, void* global_data) {
    this->target = target;
    this->global_data = global_data;
    if (this->saved_states.size() > 0) {
        for (auto& [track_name, track] : this->saved_states) {
            this->change_state_at(
                track.track_need_handle,
                track.state_need_handle,
                track.duration_state_need_handle,
                track.state_tag_need_handle
            );
        }
        this->saved_states.clear();
    }
}

void State_Machine_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
    for (auto& [track_name, track] : tracks) {
        track.current_processing_duration += delta_time;
        track.last_state_processing = track.current_state;
    }
}

void State_Machine_Component::handle_auto_change_state(
    Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
) {}

void State_Machine_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
    for (auto& [track_name, track] : tracks) {
        std::string& current_state_name = track.current_state;
        /**
         * @Note: If state is infinity the target NEED TO change it manualy!
         */
        if (track.max_state_duration == INFITY_STATE)
            continue;
        if (track.current_processing_duration >= track.max_state_duration) {
            std::string current_state = track.current_state;
            if (!track.is_callback && track.states.find(current_state) != track.states.end()) {
                State_Information& state = track.states[current_state];
                if (state.end_state != nullptr) {
                    state.end_state(this->target, this, this->global_data, state.source_call_tag);
                }
            }
            if (!track.is_callback && track.finish_callback != nullptr) {
                track.finish_callback({target, this, global_data, current_state_name});
            }
            if (!track.is_callback) {
                track.is_callback = true;
                int source_call_state = -1;
                if (track.states.find(current_state) != track.states.end()) {
                    State_Information& state = track.states[current_state];
                    source_call_state = state.source_call_tag;
                }
                this->handle_auto_change_state(target, track_name, current_state, global_data, source_call_state);
            }
            track.is_finish = true;
        }
    }
}