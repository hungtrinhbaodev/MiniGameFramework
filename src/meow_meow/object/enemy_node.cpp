#include <actions.h>
#include <defined.h>
#include <math_custom.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/const.h>
#include <meow_meow/global_data.h>
#include <meow_meow/object/enemy_node.h>
#include <state_machine_component.h>
#include <utils.h>

namespace Meow_Meow {
    Enemy_Node::Enemy_Node() {
        this->init_enemy_animation();
        this->init_components();
    }

    Enemy_Node::~Enemy_Node() {}

    Custom::Transformed_Rectangle Enemy_Node::get_bounding_box() {
        Custom::Size bounding_size = Const::ENEMY_BOUNDING_BOX;
        return Custom::Transformed_Rectangle{
            Custom::Rectangle{bounding_size.width, bounding_size.height}.apply(this->get_transform(), {0.5f, 0.5f})
        };
    }

    void Enemy_Node::handle_boundary(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        Custom::Size layer_size = battle_layer->get_content_size();
        Custom::Rectangle_Area layer_rect{0, 0, layer_size.width, layer_size.width};
        Custom::Size enemy_bounding_size = Const::ENEMY_BOUNDING_BOX;
        glm::vec2 enemy_start_box_position =
            this->get_position() - glm::vec2{0.5f, 0.5f} * enemy_bounding_size.to_vec2();
        Custom::Rectangle_Area enemy_rect = {
            enemy_start_box_position.x,
            enemy_start_box_position.y,
            enemy_bounding_size.width,
            enemy_bounding_size.height
        };
        Custom::Rectangle_Area enemy_rect_fix_with_layer = enemy_rect;
        enemy_rect.fix_with(layer_rect);
        this->set_position(
            this->get_position() - (enemy_rect_fix_with_layer.get_position() - enemy_rect.get_position())
        );
    }

    void Enemy_Node::init_enemy_animation() {
        this->enemy_animation = new Character_Animation(this->enemy_animation_id, 1);
        this->enemy_animation->set_character_id(this->enemy_animation_id);
        this->add_child(this->enemy_animation);
    }

    void Enemy_Node::init_components() {
        Enemy_Behavior_Component* behavior = new Enemy_Behavior_Component();
        behavior->set_name(Const::ENEMY_BEHAVIOR_COMPONENT_NAME);
        this->add_component(behavior);

        State_Machine_Component* state_machine = new State_Machine_Component();
        state_machine->add_track(Const::TRACK_CONTROLL, nullptr);
        state_machine->add_track(Const::TRACK_EFFECTED, nullptr);
        state_machine->set_name(Defined::COMPONENT_STATE_MACHINE_NAME);
        this->add_component(state_machine);

        this->add_key_press_listener(Custom::Key::V);
    }

    void Enemy_Node::attach(void* global_data) {
        this->enemy_animation->play_animation("IDLE");
    }

    void Enemy_Node::change_to_attack(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        behavior->start_attack_countdown();
        float animation_duration = this->enemy_animation->get_amimation_duration("ATTACK");
        this->enemy_animation->play_animation(
            "ATTACK", behavior_config.get_enemy_attack_duration() / animation_duration
        );

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_ATTACK, behavior_config.get_enemy_attack_duration()
        );
    }

    void Enemy_Node::change_to_jump(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        behavior->start_jump_countdown();
        float animation_duration = this->enemy_animation->get_amimation_duration("IDLE");
        this->enemy_animation->play_animation("IDLE", behavior_config.get_enemy_jump_duration() / animation_duration);
        this->action_enemy_jump(0.f, behavior_config.get_enemy_jump_duration(), behavior->get_jump_position());

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_JUMP, behavior_config.get_enemy_jump_duration()
        );
    }

    void Enemy_Node::change_to_walk(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        this->velosity = behavior_config.get_enemy_velosity() * behavior->get_enemy_walking_direction();

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_WALK, behavior_config.get_enemy_walk_duration()
        );

        std::string last_state = state_machine->get_last_state_processign_at(Const::TRACK_CONTROLL);
        if (last_state != Const::STATE_WALK) {
            float duration = this->enemy_animation->get_amimation_duration("WALK");
            this->enemy_animation->play_animation("WALK", behavior_config.get_enemy_walk_duration() / duration);
        }
    }

    float Enemy_Node::action_enemy_jump(float delay, float duration, glm::vec2 character_position) {
        this->stop_action(JUMP_ACTION_TAG);
        glm::vec2 start_position = this->get_position();
        glm::vec2 end_position = character_position;
        glm::vec2 delta_position = end_position - start_position;
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position, end_position, 200 + Math::random_float(0, 50), 0.5f, delta_position.x > 0 ? -1 : 1
        );
        float sign_rotation = delta_position.x > 0 ? -1 : 1;
        this->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::bezier_to(duration, middle_position, end_position, Action_Ease::SINE_IN),
                    Action::sequence(
                        Action::scale_to(duration / 2, {0.85, 0.85}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.0, 1.0}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_to(duration / 2, sign_rotation * 30, Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    )
                )
            ),
            JUMP_ACTION_TAG
        );
        return duration;
    }

    void Enemy_Node::handle_state_machine(float delta_time, void* global_data) {
        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        if (state_machine->get_current_state_at(Const::TRACK_CONTROLL) == "") {
            this->change_to_walk(global_data);
            return;
        }

        if (state_machine->is_finish_state_at(Const::TRACK_CONTROLL)) {
            if (behavior->can_attack()) {
                this->change_to_attack(global_data);
            } else if (behavior->can_jump()) {
                this->change_to_jump(global_data);
            } else {
                this->change_to_walk(global_data);
            }
        }
    }

    void Enemy_Node::update_movement(float delta_time) {
        this->set_position(this->get_position() + this->velosity * delta_time);
    }

    void Enemy_Node::update_enemy_direction() {
        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);
        bool is_right = behavior->get_enemy_walking_direction().x > 0;
        this->enemy_animation->set_flipped_x(is_right);
        glm::vec2 anchor = !is_right
                               ? ORIGIN_ANIMATION_ANCHOR_POINT.to_vec2()
                               : glm::vec2{1.0f - ORIGIN_ANIMATION_ANCHOR_POINT.x, ORIGIN_ANIMATION_ANCHOR_POINT.y};
        this->enemy_animation->set_anchor(anchor);
    }

    void Enemy_Node::on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        switch (key) {
            case Custom::V: {
                if (pressed_detail.type == Key_Input_Type::PRESSED) {
                    glm::vec2 direction = behavior->get_enemy_walking_direction();
                    glm::vec2 jump_position = direction * behavior_config.get_enemy_jump_distane();
                    this->action_enemy_jump(0, behavior_config.get_enemy_jump_duration(), jump_position);
                }
                break;
            }
        }
    }

    void Enemy_Node::fix_update(float delta_time, void* global_data) {
        this->handle_state_machine(delta_time, global_data);
        this->update_movement(delta_time);
        this->update_enemy_direction();
        Game_Object::fix_update(delta_time, global_data);
    }
}  // namespace Meow_Meow