#pragma once
#include <base_component.h>

#include <functional>
#include <map>
#include <vector>

class State_Machine_Component : public Base_Component {
public:
    struct Callback_Finish_State {
        Base_Node* target = nullptr;
        State_Machine_Component* component = nullptr;
        void* global_data = nullptr;
        std::string current_state = "";
    };

    State_Machine_Component();
    ~State_Machine_Component();

    bool is_finish_state_at(const std::string& track);
    std::string get_current_state_at(std::string track);
    std::string get_last_state_processign_at(std::string track);

    void add_track(std::string track_name, std::function<void(Callback_Finish_State)> finish_state_callback);
    void change_state_at(std::string track, std::string state, float state_duration);

protected:
    void attach(Base_Node* target, void* global_data) override;
    void update_information(Base_Node* target, float delta_time, void* global_data) override;
    void handle_task(Base_Node* target, float delta_time, void* global_data) override;

private:
    struct Track_Information {
        std::string track_name;
        std::string current_state;
        float current_processing_duration;
        float max_state_duration;
        bool is_callback = false;
        std::function<void(Callback_Finish_State)> finish_callback = nullptr;
        std::string last_state_processing = "";
        bool is_finished_state();
    };

    Base_Node* target = nullptr;
    void* global_data = nullptr;
    std::map<std::string, Track_Information> tracks;
};