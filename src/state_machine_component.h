#pragma once
#include <base_component.h>

#include <functional>
#include <iostream>
#include <map>
#include <vector>

class State_Machine_Component : public Base_Component {
public:
    struct State_Machine_Callback_Data {
        Base_Node* target = nullptr;
        State_Machine_Component* state_machine = nullptr;
        void* global_data = nullptr;
        int source_call_state = -1;
        std::string next_state = "";
        int next_state_source_call = -1;
    };
    static float INFITY_STATE;
    using Callback = std::function<void(State_Machine_Callback_Data)>;
    struct Callback_Finish_State {
        Base_Node* target = nullptr;
        State_Machine_Component* component = nullptr;
        void* global_data = nullptr;
        std::string current_state = "";
    };
    struct State_Saved_Need_Execution {
        std::string track_need_handle = "";
        std::string state_need_handle = "";
        float duration_state_need_handle = 0.f;
        int state_tag_need_handle = 0;
    };

    State_Machine_Component();
    ~State_Machine_Component();

    bool is_finish_state_at(const std::string& track);
    std::string get_current_state_at(std::string track);
    std::string get_last_state_processign_at(std::string track);
    float get_duration_state_at(std::string track);
    float get_max_duration_state_at(std::string track);
    float get_duration_remain_state_at(std::string track);
    int get_source_call_state_at(std::string track, std::string state);

    void add_track(std::string track_name, std::function<void(Callback_Finish_State)> finish_state_callback);
    void add_state_at(
        std::string track_name, std::string state_name, Callback start_state = nullptr, Callback finish_state = nullptr
    );
    virtual void change_state_at(
        std::string track,
        std::string state,
        float state_duration,
        int source_call_tag = -1,
        bool call_end_last_state = true
    );
    void log(std::string track);

protected:
    void attach(Base_Node* target, void* global_data) override;
    void update_information(Base_Node* target, float delta_time, void* global_data) override;
    void handle_task(Base_Node* target, float delta_time, void* global_data) override;
    virtual void handle_auto_change_state(
        Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
    );

    Base_Node* target = nullptr;
    void* global_data = nullptr;

private:
    struct State_Information {
        std::string state_name = "";
        Callback start_state = nullptr;
        Callback end_state = nullptr;
        int source_call_tag = -1;
    };

    struct Track_Information {
        std::string track_name;
        std::string current_state;
        float current_processing_duration;
        float max_state_duration;
        bool is_callback = false;
        std::function<void(Callback_Finish_State)> finish_callback = nullptr;
        std::string last_state_processing = "";
        bool is_finish = false;
        std::map<std::string, State_Information> states;

        friend std::ostream& operator<<(std::ostream& os, const Track_Information& track) {
            std::cout << "current_state: " << track.current_state
                      << ", current_processing_duration: " << track.current_processing_duration
                      << ", max_state_duration: " << track.max_state_duration << std::endl;
            return os;
        }
    };

    std::string state_need_handle = "";
    std::string track_need_handle = "";
    float duration_state_need_handle = 0.f;
    int state_tag_need_handle = 0;
    std::map<std::string, Track_Information> tracks;
    std::map<std::string, State_Saved_Need_Execution> saved_states;
};