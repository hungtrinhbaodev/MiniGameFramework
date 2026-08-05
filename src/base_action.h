#pragma once
#include <Base_Node.h>

enum Action_Ease {
    LINEAR,
    SINE_IN,
    SINE_OUT,
    SINE_IN_OUT
};

enum Action_Subtype {
    TO,
    BY
};

enum Action_Type {
    SEQUENCE,
    SPAWN
};

class Base_Action {
public:
    Base_Action();
    ~Base_Action();

    virtual void with_start_state(Base_Node::Transform& tranform);
    bool travle(Base_Node::Transform& transform, float delta_time);
    Base_Action* remove_chain_by_tag(int tag);
    int get_tag();

    void set_tag(int tag);
    void set_type(Action_Type type);
    void set_subtype(Action_Subtype subtype);
    void set_ease_type(Action_Ease ease);
    void set_duration(float duration);
    void push_spawn(Base_Action* action);
    void push_sequence(Base_Action* action);

protected:
    float total_duration = 0.f;
    float current_duration = 0.f;
    Action_Ease ease_type = Action_Ease::LINEAR;
    Action_Type type = Action_Type::SEQUENCE;
    Action_Subtype subtype = Action_Subtype::TO;
    int tag = -1;

    // Use in spawn action chain
    Base_Action* next_spawn_chain = nullptr;

    // Use in sequence action chain
    Base_Action* next_sequence_chain = nullptr;

    bool is_end();
    float get_rate();
    virtual void apply(Base_Node::Transform& tranform, float delta_time);
};