#include <actions.h>
#include <progression_node.h>

#include <iostream>

Action_Delay::Action_Delay() {}

Action_Delay::~Action_Delay() {}

Action_Move::Action_Move() {}

Action_Move::~Action_Move() {}

void Action_Move::set_end(glm::vec2 end) {
    end_x = end.x;
    end_y = end.y;
}

void Action_Move::set_delta(glm::vec2 delta) {
    delta_position = delta;
}

void Action_Move::setup_target_to_action(Base_Node* target) {
    if (subtype == Action_Subtype::TO) {
        Custom ::Transform transform = target->get_transform();
        delta_position.x = end_x - transform.position.x;
        delta_position.y = end_y - transform.position.y;
    }
}

void Action_Move::apply(Base_Node* target, float delta_time) {
    float last_rate = this->get_rate();
    Base_Action::apply(target, delta_time);
    float current_rate = this->get_rate();

    glm::vec2 distance = (current_rate - last_rate) * delta_position;
    target->modify_transform().position += distance;
}

Action_Scale::Action_Scale() {}

Action_Scale::~Action_Scale() {}

void Action_Scale::set_end(glm::vec2 end) {
    end_x = end.x;
    end_y = end.y;
}

void Action_Scale::set_delta(glm::vec2 delta) {
    delta_scale = delta;
}

void Action_Scale::setup_target_to_action(Base_Node* target) {
    if (subtype == Action_Subtype::TO) {
        Custom ::Transform transform = target->get_transform();
        delta_scale.x = end_x - transform.scale.x;
        delta_scale.y = end_y - transform.scale.y;
    }
}

void Action_Scale::apply(Base_Node* target, float delta_time) {
    float last_rate = this->get_rate();
    Base_Action::apply(target, delta_time);
    float current_rate = this->get_rate();

    glm::vec2 size = (current_rate - last_rate) * delta_scale;
    target->modify_transform().scale += size;
}

Action_Rotate::Action_Rotate() {}

Action_Rotate::~Action_Rotate() {}

void Action_Rotate::set_end(float end) {
    end_rotation = end;
}

void Action_Rotate::set_delta(float delta) {
    delta_rotation = delta;
}

void Action_Rotate::setup_target_to_action(Base_Node* target) {
    if (subtype == Action_Subtype::TO) {
        Custom ::Transform transform = target->get_transform();
        delta_rotation = end_rotation - transform.rotation;
    }
}

void Action_Rotate::apply(Base_Node* target, float delta_time) {
    float last_rate = this->get_rate();
    Base_Action::apply(target, delta_time);
    float current_rate = this->get_rate();

    float rotate_more = (current_rate - last_rate) * delta_rotation;
    target->modify_transform().rotation += rotate_more;
}

Action_Opacity::Action_Opacity() {}

Action_Opacity::~Action_Opacity() {}

void Action_Opacity::set_end(unsigned char end) {
    end_opacity = end;
}

void Action_Opacity::set_delta(int delta) {
    delta_opacity = delta;
}

void Action_Opacity::setup_target_to_action(Base_Node* target) {
    if (subtype == Action_Subtype::TO) {
        Custom ::Transform transform = target->get_transform();
        delta_opacity = std::max(std::min((int)end_opacity, 255), 0) - transform.opacity;
    }
}

void Action_Opacity::apply(Base_Node* target, float delta_time) {
    float last_rate = this->get_rate();
    Base_Action::apply(target, delta_time);
    float current_rate = this->get_rate();

    float opacity_more = (current_rate - last_rate) * delta_opacity;

    target->modify_transform().opacity =
        (unsigned char)std::max(std::min((int)(target->get_opacity() + opacity_more), 255), 0);
}

Action_Remove_Self::Action_Remove_Self() {}

Action_Remove_Self::~Action_Remove_Self() {}

void Action_Remove_Self::set_cleanup(bool is_cleanup) {
    this->is_cleanup = is_cleanup;
}

void Action_Remove_Self::setup_target_to_action(Base_Node* target) {}

void Action_Remove_Self::apply(Base_Node* target, float delta_time) {
    Base_Action::apply(target, delta_time);
    if (this->target != nullptr) {
        this->target->remove_from_parent(this->is_cleanup);
    }
}

Action_Visible::Action_Visible() {}

Action_Visible::~Action_Visible() {}

void Action_Visible::set_show(bool is_show) {
    this->is_show = is_show;
}

void Action_Visible::setup_target_to_action(Base_Node* target) {}

void Action_Visible::apply(Base_Node* target, float delta_time) {
    Base_Action::apply(target, delta_time);
    if (this->target != nullptr) {
        this->target->set_visible(this->is_show);
    }
}

Action_Progression::Action_Progression() {}

Action_Progression::~Action_Progression() {}

void Action_Progression::set_end(float end) {
    end_progression = end;
}

void Action_Progression::set_delta(float delta) {
    delta_progression = delta;
}

void Action_Progression::setup_target_to_action(Base_Node* target) {
    if (subtype == Action_Subtype::TO) {
        Progression_Node* progression = reinterpret_cast<Progression_Node*>(target);
        delta_progression = end_progression - progression->get_percent();
    }
}

void Action_Progression::apply(Base_Node* target, float delta_time) {
    float last_rate = this->get_rate();
    Base_Action::apply(target, delta_time);
    float current_rate = this->get_rate();

    Progression_Node* progression = reinterpret_cast<Progression_Node*>(target);
    float progression_more = (current_rate - last_rate) * delta_progression;
    progression->set_percent(progression->get_percent() + progression_more);
}

bool Action_Progression::is_valid_target(Base_Node* target) {
    return target->get_type() == Node_Type::PROGRESSION;
}

Action_Callback::Action_Callback() {}

Action_Callback::~Action_Callback() {}

void Action_Callback::set_caller(std::function<void(Base_Node*, void*)> caller) {
    this->caller = caller;
}

void Action_Callback::apply(Base_Node* target, float delta_time) {
    Base_Action::apply(target, delta_time);
    if (target != nullptr && caller != nullptr) {
        caller(target, this->global_data);
    }
}

Action_Delay* Action::delay(float delay_time) {
    Action_Delay* delay = new Action_Delay();
    delay->set_duration(delay_time);
    return delay;
}

Action_Move* Action::move_to(float duration, glm::vec2 position_to, Action_Ease ease) {
    Action_Move* move = new Action_Move();
    move->set_subtype(Action_Subtype::TO);
    move->set_ease_type(ease);
    move->set_end(position_to);
    move->set_duration(duration);
    return move;
}

Action_Move* Action::move_to(float duration, float x, float y, Action_Ease ease) {
    return Action::move_to(duration, {x, y}, ease);
}

Action_Move* Action::move_by(float duration, glm::vec2 position_by, Action_Ease ease) {
    Action_Move* move = new Action_Move();
    move->set_subtype(Action_Subtype::BY);
    move->set_ease_type(ease);
    move->set_delta(position_by);
    move->set_duration(duration);
    return move;
}

Action_Move* Action::move_by(float duration, float x, float y, Action_Ease ease) {
    return Action::move_by(duration, {x, y}, ease);
}

Action_Scale* Action::scale_to(float duration, glm::vec2 scale_to, Action_Ease ease) {
    Action_Scale* scale = new Action_Scale();
    scale->set_subtype(Action_Subtype::TO);
    scale->set_ease_type(ease);
    scale->set_end(scale_to);
    scale->set_duration(duration);
    return scale;
}

Action_Scale* Action::scale_to(float duration, float x, float y, Action_Ease ease) {
    return Action::scale_to(duration, {x, y}, ease);
}

Action_Scale* Action::scale_by(float duration, glm::vec2 scale_by, Action_Ease ease) {
    Action_Scale* scale = new Action_Scale();
    scale->set_subtype(Action_Subtype::BY);
    scale->set_ease_type(ease);
    scale->set_delta(scale_by);
    scale->set_duration(duration);
    return scale;
}

Action_Scale* Action::scale_by(float duration, float x, float y, Action_Ease ease) {
    return Action::scale_by(duration, {x, y}, ease);
}

Action_Rotate* Action::rotate_to(float duration, float rotate_to, Action_Ease ease) {
    Action_Rotate* rotation = new Action_Rotate();
    rotation->set_subtype(Action_Subtype::TO);
    rotation->set_ease_type(ease);
    rotation->set_end(rotate_to);
    rotation->set_duration(duration);
    return rotation;
}

Action_Rotate* Action::rotate_by(float duration, float rotate_by, Action_Ease ease) {
    Action_Rotate* rotation = new Action_Rotate();
    rotation->set_subtype(Action_Subtype::BY);
    rotation->set_ease_type(ease);
    rotation->set_delta(rotate_by);
    rotation->set_duration(duration);
    return rotation;
}

Action_Opacity* Action::fade_to(float duration, unsigned char opacity_to, Action_Ease ease) {
    Action_Opacity* opacity = new Action_Opacity();
    opacity->set_subtype(Action_Subtype::TO);
    opacity->set_ease_type(ease);
    opacity->set_end(opacity_to);
    opacity->set_duration(duration);
    return opacity;
}

Action_Opacity* Action::fade_in(float duration, Action_Ease ease) {
    return Action::fade_to(duration, 255, ease);
}

Action_Opacity* Action::fade_out(float duration, Action_Ease ease) {
    return Action::fade_to(duration, 0, ease);
}

Action_Remove_Self* Action::remove_self(bool is_cleanup) {
    Action_Remove_Self* remove = new Action_Remove_Self();
    remove->set_cleanup(is_cleanup);
    remove->set_type(Action_Type::ALWAY_HAPPEN);
    return remove;
}

Action_Visible* Action::show() {
    Action_Visible* visible = new Action_Visible();
    visible->set_show(true);
    visible->set_type(Action_Type::ALWAY_HAPPEN);
    return visible;
}

Action_Visible* Action::hide() {
    Action_Visible* visible = new Action_Visible();
    visible->set_show(false);
    visible->set_type(Action_Type::ALWAY_HAPPEN);
    return visible;
}

Action_Progression* Action::progress_to(float duration, float progression_to, Action_Ease ease) {
    Action_Progression* progression = new Action_Progression();
    progression->set_subtype(Action_Subtype::TO);
    progression->set_ease_type(ease);
    progression->set_end(progression_to);
    progression->set_duration(duration);
    return progression;
}

Action_Progression* Action::progress_by(float duration, float progression_by, Action_Ease ease) {
    Action_Progression* progression = new Action_Progression();
    progression->set_subtype(Action_Subtype::BY);
    progression->set_ease_type(ease);
    progression->set_delta(progression_by);
    progression->set_duration(duration);
    return progression;
}

Action_Callback* Action::call_func(std::function<void(Base_Node*, void*)> caller) {
    Action_Callback* callback = new Action_Callback();
    callback->set_caller(caller);
    callback->set_type(Action_Type::ALWAY_HAPPEN);
    return callback;
}

Base_Action* Action::sequence(std::vector<Base_Action*> actions) {
    Base_Action* first = nullptr;
    if (actions.size() > 0) {
        first = actions.front();
    } else {
        first = new Base_Action();
    }
    for (int i = 1; i < actions.size(); i++) {
        first->push_sequence(actions[i]);
    }
    return first;
}

Base_Action* Action::spawn(std::vector<Base_Action*> actions) {
    Base_Action* first = nullptr;
    if (actions.size() > 0) {
        first = actions.front();
    } else {
        first = new Base_Action();
    }
    for (int i = 1; i < actions.size(); i++) {
        first->push_spawn(actions[i]);
    }
    return first;
}