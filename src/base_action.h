#pragma once
#include <base_node.h>
#include <custom.h>

#include <iostream>

enum Action_Ease { LINEAR, SINE_IN, SINE_OUT };

enum Action_Subtype { TO, BY };

enum Action_Type { ALWAY_HAPPEN, TIMEER };

class Base_Action {
public:
    struct Action_Processing_Information {
        float start_time = 0.f;
        float end_time = 0.f;
        std::string action_name = "";
        friend std::ostream& operator<<(std::ostream& os, const Action_Processing_Information& debug) {
            std::cout << "Action information: " << debug.action_name << ", start time: " << debug.start_time
                      << ", end time: " << debug.end_time << std::endl;
            return os;
        }
        bool operator<(const Action_Processing_Information& other) {
            if (start_time == other.start_time) {
                return end_time < other.end_time;
            }
            return start_time < other.start_time;
        }
    };

    Base_Action();
    ~Base_Action();

    void assign_target_to_all_chain(Base_Node* target, void* global_data, bool debug = false);
    void assign_target_2(Base_Node* target, void* global_data, bool debug = false);
    bool travel_action(Base_Node* target, float delta_time, void* global_data);
    bool travel_action_2(Base_Node* target, float delta_time, void* global_data);
    int get_tag();

    void set_tag(int tag);
    void set_type(Action_Type type);
    void set_subtype(Action_Subtype subtype);
    void set_ease_type(Action_Ease ease);
    void set_duration(float duration);
    void set_global_data(void* global_data);
    void set_debug(bool debug);
    void set_removed(bool removed);

    void push_spawn(Base_Action* action);
    void push_sequence(Base_Action* action);
    Base_Action* repeat(int repeat);
    Base_Action* repeat_forever();
    bool is_removed();
    bool is_debug();
    void show_debug();

    bool sequence(
        Base_Node* target,
        float delta_time,
        void* global_data,
        std::vector<Action_Processing_Information>& processing_informations,
        long& start_time_chain,
        bool debug = false
    );
    bool spawn(
        Base_Node* target,
        float delta_time,
        void* global_data,
        std::vector<Action_Processing_Information>& processing_informations,
        long& start_time_chain,
        bool debug = false
    );

protected:
    Base_Node* target = nullptr;
    void* global_data = nullptr;
    float total_duration = 0.f;
    float current_duration = 0.f;
    Action_Ease ease_type = Action_Ease::LINEAR;
    Action_Type type = Action_Type::TIMEER;
    Action_Subtype subtype = Action_Subtype::TO;
    int tag = -1;
    bool is_setup = false;

    /**
     * Use in spawn action chain
     * */
    Base_Action* next_spawn_chain = nullptr;

    /**
     * Use in sequence action chain
     * */
    Base_Action* next_sequence_chain = nullptr;

    bool is_end();
    float get_rate();
    void deep_clean();
    virtual bool is_valid_target(Base_Node* target);
    virtual void setup_target_to_action(Base_Node* target);
    virtual bool update_action(
        Base_Node* target,
        float delta_time,
        void* global_data,
        std::vector<Action_Processing_Information>& processing_informations,
        long& start_time_chain,
        bool debug = false
    );
    virtual void apply(Base_Node* target, float delta_time);
    virtual std::string get_action_name();

private:
    bool is_repeat_forever = false;
    /**
     * With action use once we apply once
     * */
    bool is_appled = false;
    int repeat_time = 0;
    bool removed = false;

    /**
     * Debug session save all information of sequence and spawn action do in action chain
     * */
    float start_chain_time = 0.f;
    std::vector<Action_Processing_Information> processing_informations;
    bool debug = false;
    bool added_into_processing_informations = false;

    bool travel(
        Base_Node* target,
        float delta_time,
        void* global_data,
        std::vector<Action_Processing_Information>& processing_informations,
        long start_time_chain = -1,
        bool debug = false
    );
    void recycle();
};