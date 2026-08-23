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
    Enemy_Node::Enemy_Node() {}

    Enemy_Node::~Enemy_Node() {}

    Custom::Transformed_Rectangle Enemy_Node::get_bounding_box() {
        return {};
    }

    void Enemy_Node::handle_boundary(void* global_data) {}

    void Enemy_Node::init_enemy_animation() {
        this->enemy_animation = new Character_Animation();
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
        this->effect_enemy_jump(0.f, behavior_config.get_enemy_jump_duration(), behavior->get_jump_position());

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_JUMP, behavior_config.get_enemy_attack_duration()
        );
    }

    void Enemy_Node::change_to_walk(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const auto& behavior_config = data->get_config().get_enemy_behavior_config();

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(this, Defined::COMPONENT_STATE_MACHINE_NAME);

        Enemy_Behavior_Component* behavior =
            Utils::get_component<Enemy_Behavior_Component>(this, Const::ENEMY_BEHAVIOR_COMPONENT_NAME);

        glm::vec2 direction = behavior->get_enemy_walking_direction();
        this->velosity = behavior_config.get_enemy_velosity() * direction;
        this->direction = direction;

        state_machine->change_state_at(
            Const::TRACK_CONTROLL, Const::STATE_WALK, behavior_config.get_enemy_walk_duration()
        );

        std::string last_state = state_machine->get_last_state_processign_at(Const::TRACK_CONTROLL);
        if (last_state != Const::STATE_WALK) {
            this->enemy_animation->play_animation("WALK");
        }
    }

    float Enemy_Node::effect_enemy_jump(float delay, float duration, glm::vec2 character_position) {
        this->stop_action(JUMP_ACTION_TAG);
        glm::vec2 start_position = this->get_position();
        glm::vec2 end_position = character_position;
        glm::vec2 delta_position = end_position - start_position;
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position, end_position, 200 + Math::random_float(0, 50), 0.5f, delta_position.x > 0 ? -1 : 1
        );
        float sign_rotation = delta_position.x > 0 ? 1 : -1;
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

    void Enemy_Node::fix_update(float delta_time, void* global_data) {
        this->handle_state_machine(delta_time, global_data);
    }
}  // namespace Meow_Meow