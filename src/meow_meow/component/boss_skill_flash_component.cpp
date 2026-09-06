#include <math_custom.h>
#include <meow_meow/component/boss_skill_flash_component.h>
#include <meow_meow/config/boss_skill_flash_config.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/data/global_data.h>
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

        State_Machine_Component* player_state_machine =
            Utils::get_component<State_Machine_Component>(character, Defined::COMPONENT_STATE_MACHINE_NAME);
        if (player_state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH) {
            return false;
        }

        const Boss_Skill_Flash_Config& skill_config = data->get_config().get_boss_skill_flash_config();
        if (state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED ||
            state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN) {
            return false;
        }
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