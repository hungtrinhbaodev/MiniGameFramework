#pragma once
#include <base_node.h>
#include <custom.h>

enum Action_Ease { LINEAR, SINE_IN, SINE_OUT };

enum Action_Subtype { TO, BY };

enum Action_Type { ALWAY_HAPPEN, TIMEER };

class Base_Action {
public:
    Base_Action();
    ~Base_Action();

    void with_start_state(Custom::Transform& tranform, Base_Node* target);
    bool travel(Custom::Transform& transform, float delta_time);
    int get_tag();

    void set_tag(int tag);
    void set_type(Action_Type type);
    void set_subtype(Action_Subtype subtype);
    void set_ease_type(Action_Ease ease);
    void set_duration(float duration);
    void set_target(Base_Node* target);

    void push_spawn(Base_Action* action);
    void push_sequence(Base_Action* action);
    Base_Action* repeat(int repeat);
    Base_Action* repeat_forever();

protected:
    Base_Node* target = nullptr;
    float total_duration = 0.f;
    float current_duration = 0.f;
    Action_Ease ease_type = Action_Ease::LINEAR;
    Action_Type type = Action_Type::TIMEER;
    Action_Subtype subtype = Action_Subtype::TO;
    int tag = -1;
    bool is_setup = false;

    // Use in spawn action chain
    Base_Action* next_spawn_chain = nullptr;

    // Use in sequence action chain
    Base_Action* next_sequence_chain = nullptr;

    bool is_end();
    float get_rate();
    void deep_clean();

    virtual void setup_start_state(Custom::Transform& transform);
    virtual void apply(Custom::Transform& tranform, float delta_time);

private:
    bool is_repeat_forever = false;
    // with action use once we apply once
    bool is_appled = false;
    int repeat_time = 0;

    void recycle();
};