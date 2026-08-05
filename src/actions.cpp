#include <actions.h>

Action_Move::Action_Move() {
}

Action_Move::~Action_Move() {
}

void Action_Move::set_end(glm::vec2 end) {
    end_x = end.x;
    end_y = end.y;
}

void Action_Move::set_delta(glm::vec2 delta) {
    delta_position = delta;
}

void Action_Move::with_start_state(Base_Node::Transform& transform) {
    if (subtype == Action_Subtype::TO) {
        delta_position.x = end_x - transform.position.x;
        delta_position.x = end_y - transform.position.y;
    }
}

void Action_Move::apply(Base_Node::Transform& tranform, float delta_time) {
    float last_rate = get_rate();
    Base_Action::apply(tranform, delta_time);
    float current_rate = get_rate();

    glm::vec2 current = (current_rate - last_rate) * delta_position;
    tranform.position += current;
}

Action_Scale::Action_Scale() {
}

Action_Scale::~Action_Scale() {
}

void Action_Scale::set_end(glm::vec2 end) {
    end_x = end.x;
    end_y = end.y;
}

void Action_Scale::set_delta(glm::vec2 delta) {
    delta_scale = delta;
}

void Action_Scale::with_start_state(Base_Node::Transform& transform) {
    if (subtype == Action_Subtype::TO) {
        delta_scale.x = end_x - transform.scale.x;
        delta_scale.x = end_y - transform.scale.y;
    }
}

void Action_Scale::apply(Base_Node::Transform& tranform, float delta_time) {
    float last_rate = get_rate();
    Base_Action::apply(tranform, delta_time);
    float current_rate = get_rate();

    glm::vec2 current = (current_rate - last_rate) * delta_scale;
    tranform.scale += current;
}

Action_Move* Actions::move_to(float duration, glm::vec2 position_to, Action_Ease ease) {
    Action_Move* move = new Action_Move();
    move->set_subtype(Action_Subtype::TO);
    move->set_ease_type(ease);
    move->set_end(position_to);
    move->set_duration(duration);
    return move;
}

Action_Move* Actions::move_to(float duration, float x, float y, Action_Ease ease) {
    return Actions::move_to(duration, {x, y}, ease);
}

Action_Move* Actions::move_by(float duration, glm::vec2 position_by, Action_Ease ease) {
    Action_Move* move = new Action_Move();
    move->set_subtype(Action_Subtype::BY);
    move->set_ease_type(ease);
    move->set_delta(position_by);
    move->set_duration(duration);
    return move;
}

Action_Move* Actions::move_by(float duration, float x, float y, Action_Ease ease) {
    return Actions::move_by(duration, {x, y}, ease);
}

Action_Scale* Actions::scale_to(float duration, glm::vec2 scale_to, Action_Ease ease) {
    Action_Scale* scale = new Action_Scale();
    scale->set_subtype(Action_Subtype::TO);
    scale->set_ease_type(ease);
    scale->set_end(scale_to);
    scale->set_duration(duration);
    return scale;
}

Action_Scale* Actions::scale_to(float duration, float x, float y, Action_Ease ease) {
    return Actions::scale_to(duration, {x, y}, ease);
}

Action_Scale* Actions::scale_by(float duration, glm::vec2 scale_by, Action_Ease ease) {
    Action_Scale* scale = new Action_Scale();
    scale->set_subtype(Action_Subtype::BY);
    scale->set_ease_type(ease);
    scale->set_delta(scale_by);
    scale->set_duration(duration);
    return scale;
}

Action_Scale* Actions::scale_by(float duration, float x, float y, Action_Ease ease) {
    return Actions::scale_by(duration, {x, y}, ease);
}

Base_Action* Actions::sequence(std::vector<Base_Action*> actions) {
    Base_Action* first = nullptr;
    if (actions.size() > 0) {
        first = actions[1];
    } else {
        first = new Base_Action();
    }
    for (int i = 1; i < actions.size(); i++) {
        first->push_sequence(actions[i]);
    }
    return first;
}

Base_Action* Actions::spawn(std::vector<Base_Action*> actions) {
    Base_Action* first = nullptr;
    if (actions.size() > 0) {
        first = actions[1];
    } else {
        first = new Base_Action();
    }
    for (int i = 1; i < actions.size(); i++) {
        first->push_spawn(actions[i]);
    }
    return first;
}