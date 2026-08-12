#pragma once
#include <base_action.h>

#include <type_traits>

class Action_Move : public Base_Action {
public:
    Action_Move();
    ~Action_Move();

    void set_end(glm::vec2 end);
    void set_delta(glm::vec2 delta);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

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

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

private:
    float end_x, end_y;
    glm::vec2 delta_scale;
};

class Action_Rotate : public Base_Action {
public:
    Action_Rotate();
    ~Action_Rotate();

    void set_end(float rotation);
    void set_delta(float delta);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

private:
    float end_rotation, delta_rotation;
};

class Action_Opacity : public Base_Action {
public:
    Action_Opacity();
    ~Action_Opacity();

    void set_end(unsigned char opacity);
    void set_delta(int delta);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

private:
    unsigned char end_opacity;
    int delta_opacity;
};

class Action_Remove_Self : public Base_Action {
public:
    Action_Remove_Self();
    ~Action_Remove_Self();

    void set_cleanup(bool is_cleanup);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

private:
    bool is_cleanup = true;
};

class Action_Visible : public Base_Action {
public:
    Action_Visible();
    ~Action_Visible();

    void set_show(bool is_show);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;

private:
    bool is_show = true;
};

class Action_Progression : public Base_Action {
public:
    Action_Progression();
    ~Action_Progression();

    void set_end(float end_progression);
    void set_delta(float delta_progression);

    void setup_target_to_action(Base_Node* target) override;
    void apply(Base_Node* target, float delta_time) override;
    bool is_valid_target(Base_Node* target) override;

private:
    float end_progression;
    float delta_progression;
};

class Action {
public:
    static Action_Move* move_to(float duration, glm::vec2 position_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Move* move_to(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Move* move_by(float duration, glm::vec2 position_by, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Move* move_by(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Scale* scale_to(float duration, glm::vec2 scale_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Scale* scale_to(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Scale* scale_by(float duration, glm::vec2 scale_by, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Scale* scale_by(float duration, float x, float y, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Rotate* rotate_to(float duration, float rotate_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Rotate* rotate_by(float duration, float scale_by, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Opacity* fade_to(float duration, unsigned char opacity_to, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Opacity* fade_in(float duration, Action_Ease ease = Action_Ease::LINEAR);
    static Action_Opacity* fade_out(float duration, Action_Ease ease = Action_Ease::LINEAR);

    static Action_Remove_Self* remove_self(bool is_cleanup);

    static Action_Visible* show();
    static Action_Visible* hide();

    static Action_Progression* progress_to(
        float duration, float progression_to, Action_Ease ease = Action_Ease::LINEAR
    );
    static Action_Progression* progress_by(
        float duration, float progression_by, Action_Ease ease = Action_Ease::LINEAR
    );

    static Base_Action* sequence(std::vector<Base_Action*> actions);
    static Base_Action* spawn(std::vector<Base_Action*> actions);

    template <typename... Args>
    static Base_Action* sequence(Args*... actions) {
        static_assert(
            (std::is_base_of_v<Base_Action, Args> && ...),
            "Actions::sequence: all arguments must derive from Base_Action"
        );
        return sequence(std::vector<Base_Action*>{actions...});
    }

    template <typename... Args>
    static Base_Action* spawn(Args*... actions) {
        static_assert(
            (std::is_base_of_v<Base_Action, Args> && ...), "Actions::spawn: all arguments must derive from Base_Action"
        );
        return spawn(std::vector<Base_Action*>{actions...});
    }
};