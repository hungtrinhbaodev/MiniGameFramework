#include <math_custom.h>
#include <meow_meow/component/enemy_jump_skill_component.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>
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

    void Enemy_Jump_Skill_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Enemy_Skill_Jump_Config& skill_config = data->get_config().get_enemy_skill_jump_config();
        Enemy_State_Machine_Component* state_machine = get_enemy_state_machine_component(target);
        if (state_machine == nullptr)
            return;
        if (this->can_activate_skill(state_machine, global_data)) {
            this->activating_skill(global_data);
            state_machine->change_state_at(Const::TRACK_CONTROLL, Const::STATE_JUMP, skill_config.duration_jump);
        }
        Skill_Component::handle_task(target, delta_time, global_data);
    }

    bool Enemy_Jump_Skill_Component::can_activate_skill(State_Machine_Component* state_machine, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);

        Character_Node* character = data->get_character_node();
        if (character == nullptr) {
            return false;
        }

        Character_State_Machine_Component* player_state_machine = get_character_state_machine(character);
        if (player_state_machine->is_character_dead()) {
            return false;
        }

        Enemy_State_Machine_Component* enemy_state_machine =
            reinterpret_cast<Enemy_State_Machine_Component*>(state_machine);
        if (enemy_state_machine->is_enemy_attacked() || enemy_state_machine->is_enemy_stun() ||
            enemy_state_machine->is_enemy_dead()) {
            return false;
        }

        const Enemy_Skill_Jump_Config& skill_config = data->get_config().get_enemy_skill_jump_config();
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