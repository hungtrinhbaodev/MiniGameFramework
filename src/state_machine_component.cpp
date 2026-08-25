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

void State_Machine_Component::add_track(
    std::string track_name, std::function<void(Callback_Finish_State)> finish_state_callback
) {
    if (this->tracks.find(track_name) == this->tracks.end()) {
        this->tracks[track_name] = {};
        this->tracks[track_name].finish_callback = finish_state_callback;
    }
}

void State_Machine_Component::change_state_at(std::string track_name, std::string state_name, float state_duration) {
    if (this->tracks.find(track_name) == this->tracks.end())
        return;
    Track_Information& track = this->tracks[track_name];
    track.current_state = state_name;
    track.current_processing_duration = 0.f;
    track.max_state_duration = state_duration;
    track.is_callback = false;
    track.is_finish = false;
}

void State_Machine_Component::log(std::string track) {
    if (this->tracks.find(track) == this->tracks.end())
        return;
    std::cout << "Track name: " << track << ", " << this->tracks[track];
}

void State_Machine_Component::attach(Base_Node* target, void* global_data) {
    this->target = target;
    this->global_data = global_data;
}

void State_Machine_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
    for (auto& [track_name, track] : tracks) {
        track.current_processing_duration += delta_time;
        track.last_state_processing = track.current_state;
    }
}

void State_Machine_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
    for (auto& [track_name, track] : tracks) {
        std::string& current_state_name = track.current_state;
        /**
         * @Note: If state is infinity the target NEED TO change it manualy!
         */
        if (track.max_state_duration == INFITY_STATE)
            continue;
        if (track.current_processing_duration >= track.max_state_duration) {
            if (!track.is_callback && track.finish_callback != nullptr) {
                track.finish_callback({target, this, global_data, current_state_name});
            }
            track.is_finish = true;
            track.is_callback = true;
        }
    }
}