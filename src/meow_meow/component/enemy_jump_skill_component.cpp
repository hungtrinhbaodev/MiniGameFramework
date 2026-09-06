#include <math_custom.h>
#include <meow_meow/component/enemy_jump_skill_component.h>
#include <meow_meow/data/global_data.h>
#include <utils.h>

namespace Meow_Meow {
    Enemy_Jump_Skill_Component::Enemy_Jump_Skill_Component() {}

    Enemy_Jump_Skill_Component::~Enemy_Jump_Skill_Component() {}

    void Enemy_Jump_Skill_Component::attach(Base_Node* target, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Enemy_Skill_Jump_Config& skill_config = data->get_config().get_enemy_skill_jump_config();
        this->set_skill_id(skill_config.skill_id);
        this->set_countdown_time(skill_config.duration_countdown);
    }

    void Enemy_Jump_Skill_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Character_Node* character = data->get_character_node();
        if (character == nullptr)
            return;
        this->player_position = character->get_position();
        this->enemy_position = target->get_position();
        Skill_Component::update_information(target, delta_time, global_data);
    }

    bool Enemy_Jump_Skill_Component::can_activate_skill(State_Machine_Component* state_machine, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        Character_Node* character = data->get_character_node();
        if (character == nullptr) {
            return false;
        }

        State_Machine_Component* player_state_machine =
            Utils::get_component<State_Machine_Component>(character, Defined::COMPONENT_STATE_MACHINE_NAME);
        if (player_state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH) {
            return false;
        }

        const Enemy_Skill_Jump_Config& skill_config = data->get_config().get_enemy_skill_jump_config();
        if (state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED ||
            state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN) {
            return false;
        }
        if (glm::distance(this->player_position, this->enemy_position) > skill_config.jump_distance) {
            return false;
        }
        return Skill_Component::can_activate_skill(state_machine, global_data);
    }

    void Enemy_Jump_Skill_Component::activating_skill(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Enemy_Behavior_Config& behavior_config = data->get_config().get_enemy_behavior_config();
        this->jump_position =
            this->player_position +
            glm::vec2{
                Math::random_float(
                    -behavior_config.get_emeny_attack_ranage(), behavior_config.get_emeny_attack_ranage()
                ),
                Math::random_float(
                    -behavior_config.get_emeny_attack_ranage(), behavior_config.get_emeny_attack_ranage()
                )
            };
        Skill_Component::activating_skill(global_data);
    }

    glm::vec2 Enemy_Jump_Skill_Component::get_jump_position() {
        return this->jump_position;
    }
}  // namespace Meow_Meow