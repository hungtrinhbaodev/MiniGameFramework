#pragma once
#include <base_action.h>

class Action_Move : public Base_Action {
public:
    Action_Move();
    ~Action_Move();

    void set_end(glm::vec2 end);
    void set_delta(glm::vec2 delta);

    void with_start_state(Base_Node::Transform& tranform) override;
    void apply(Base_Node::Transform& tranform, float delta_time) override;

private:
    float end_x, end_y;
    glm::vec2 delta_position;
};

class Action_Scale : public Base_Action {
public:
    Action_Scale();
    ~Action_Scale();

    void set_end(glm::vec2 start);
    void set_delta(glm::vec2 delta);

    void with_start_state(Base_Node::Transform& tranform) override;
    void apply(Base_Node::Transform& tranform, float delta_time) override;

private:
    float end_x, end_y;
    glm::vec2 delta_scale;
};

class Actions {
public:
    static Action_Move* move_to(float duration, glm::vec2 position_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Move* move_to(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Move* move_by(float duration, glm::vec2 position_by, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Move* move_by(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Scale* scale_to(float duration, glm::vec2 scale_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Scale* scale_to(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Scale* scale_by(float duration, glm::vec2 scale_by, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Scale* scale_by(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Base_Action* sequence(std::vector<Base_Action*> actions);
    static Base_Action* spawn(std::vector<Base_Action*> actions);
};