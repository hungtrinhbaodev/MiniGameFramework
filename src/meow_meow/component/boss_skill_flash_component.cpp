#include <math_custom.h>
#include <meow_meow/component/boss_skill_flash_component.h>
#include <meow_meow/config/boss_skill_flash_config.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>
#include <utils.h>

namespace Meow_Meow {
    Boss_Skill_Flash_Component::Boss_Skill_Flash_Component() {}

    Boss_Skill_Flash_Component::~Boss_Skill_Flash_Component() {}

    bool Boss_Skill_Flash_Component::can_activate_skill(State_Machine_Component* state_machine, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Character_Node* character = data->get_character_node();

        if (character == nullptr) {
            return false;
        }

        Character_State_Machine_Component* player_state_machine = get_character_state_machine(character);
        if (player_state_machine->is_character_dead()) {
            return false;
        }

        Boss_State_Machine_Component* boss_state_machine =
            reinterpret_cast<Boss_State_Machine_Component*>(state_machine);
        if (boss_state_machine->is_enemy_attacked() || boss_state_machine->is_enemy_attacked() ||
            boss_state_machine->is_enemy_dead() || boss_state_machine->is_using_some_skill(global_data)) {
            return false;
        }

        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
        if (glm::distance(this->player_position, this->boss_position) > skill_config.flash_distance) {
            return false;
        }

        return Skill_Component::can_activate_skill(state_machine, global_data);
    }

    void Boss_Skill_Flash_Component::activating_skill(void* global_data) {
        Skill_Component::activating_skill(global_data);
    }

    glm::vec2 Boss_Skill_Flash_Component::get_flash_to_position() {
        float range = this->boss_attack_distance;
        return glm::vec2{
            this->player_position.x + Math::random_float(-range, range),
            this->player_position.y + Math::random_float(-range, range)
        };
    }

    void Boss_Skill_Flash_Component::attach(Base_Node* target, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
        const Enemy_Behavior_Config& behavior_config = data->get_config().get_boss_behavior_config();
        this->set_skill_id(skill_config.skill_id);
        this->set_countdown_time(skill_config.duration_countdown);
        this->boss_attack_distance = behavior_config.get_emeny_attack_ranage();
    }

    void Boss_Skill_Flash_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
        Boss_State_Machine_Component* state_machine = get_boss_state_machine_component(target);
        if (state_machine == nullptr)
            return;
        if (this->is_countdown_finish() && this->can_activate_skill(state_machine, global_data)) {
            this->activating_skill(global_data);
            state_machine->change_state_at(
                Const::TRACK_CONTROLL,
                Const::STATE_SKILL_CHANNELLING,
                skill_config.duration_channeling,
                Const::BOSS_CHANNELING_FROM_SKILL_FLASH
            );
        }
        Skill_Component::handle_task(target, delta_time, global_data);
    }

    void Boss_Skill_Flash_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Character_Node* character = data->get_character_node();
        if (character == nullptr)
            return;
        this->player_position = character->get_position();
        this->boss_position = target->get_position();
        Skill_Component::update_information(target, delta_time, global_data);
    }

}  // namespace Meow_Meow