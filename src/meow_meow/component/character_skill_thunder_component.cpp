#include <meow_meow/component/character_skill_thunder_component.h>
#include <meow_meow/config/character_skill_thunder_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_battle.h>
#include <meow_meow/object/character_node.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Character_Skill_Thunder_Component::Character_Skill_Thunder_Component() {}

    Character_Skill_Thunder_Component::~Character_Skill_Thunder_Component() {}

    bool Character_Skill_Thunder_Component::can_activate_skill(
        State_Machine_Component* state_machine, void* global_data
    ) {
        if (state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_ATTACKED ||
            state_machine->get_current_state_at(Const::TRACK_EFFECTED) == Const::STATE_STUN ||
            state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DASHING ||
            state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_SKILL_CHANNELLING ||
            state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_FLIGHT) {
            return false;
        }
        return Skill_Component::can_activate_skill(state_machine, global_data);
    }

    bool Character_Skill_Thunder_Component::is_enemy_taken(int enemy_id) {
        for (int current_enemy_id : this->enemy_taken_ids) {
            if (current_enemy_id == enemy_id) {
                return true;
            }
        }
        return false;
    }

    void Character_Skill_Thunder_Component::activating_skill(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_Thunder_Config& skill_config = data->get_config().get_character_skill_thunder_config();
        Character_Node* character = data->get_character_node();
        Battle_Layer* battle_layer = data->get_battle_layer();
        enemy_taken_ids.clear();
        Skill_Component::activating_skill(global_data);
        if (character == nullptr || battle_layer == nullptr) {
            return;
        }
        float radius_taken = skill_config.radius_taken;
        int number_enemy_taken = skill_config.number_taken;
        std::vector<Enemy_Node*> enemies = battle_layer->get_enemy_nodes();
        glm::vec2 character_position = character->get_position();
        for (Enemy_Node* enemy : enemies) {
            glm::vec2 enemy_position = enemy->get_position();
            int enemy_id = enemy->get_enemy_id();
            Enemy_Data enemy_data = data->get_enemy_data_by(enemy_id);
            Enemy_State_Machine_Component* enemy_state_machine = get_enemy_state_machine_component(enemy);
            if (enemy_state_machine->is_enemy_dead() || enemy_state_machine->is_enemy_lost_all_health())
                continue;
            if (glm::distance(character_position, enemy_position) <= radius_taken && !enemy_data.is_dead()) {
                this->enemy_taken_ids.push_back(enemy_id);
                number_enemy_taken--;
            }
            if (number_enemy_taken <= 0) {
                break;
            }
        }
    }

    void Character_Skill_Thunder_Component::attach(Base_Node* target, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_Thunder_Config& skill_config = data->get_config().get_character_skill_thunder_config();
        this->set_skill_id(skill_config.skill_id);
        this->set_countdown_time(skill_config.duration_countdown);
    }

    void Character_Skill_Thunder_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        this->enemy_taken_ids.clear();
        Skill_Component::update_information(target, delta_time, global_data);
    }

}  // namespace Meow_Meow