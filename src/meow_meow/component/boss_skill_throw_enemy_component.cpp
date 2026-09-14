#include <math_custom.h>
#include <meow_meow/component/boss_skill_throw_enemy_component.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    Boss_Skill_Throw_Enemy_Component::Boss_Skill_Throw_Enemy_Component() {}

    Boss_Skill_Throw_Enemy_Component::~Boss_Skill_Throw_Enemy_Component() {}

    void Boss_Skill_Throw_Enemy_Component::attach(Base_Node* target, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Throw_Enemy_Config& skill_config = data->get_config().get_boss_skill_throw_enemy_config();
        this->set_skill_id(skill_config.skill_id);
        this->set_countdown_time(skill_config.duration_countdown);
        this->delta_end_position = skill_config.random_delta_end_range;
    }

    void Boss_Skill_Throw_Enemy_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        if (!this->is_countdown_finish())
            return;
        Boss_State_Machine_Component* boss_state_machine = get_boss_state_machine_component(target);
        if (this->can_activate_skill(boss_state_machine, global_data)) {
            Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
            const Boss_Skill_Throw_Enemy_Config& skill_config = data->get_config().get_boss_skill_throw_enemy_config();
            boss_state_machine->change_state_at(
                Const::TRACK_CONTROLL,
                Const::STATE_SKILL_CHANNELLING,
                skill_config.duration_channelling,
                Const::BOSS_CHANNELING_FROM_SKILL_THROW_ENEMY
            );
            this->chosen_enemy = this->find_suitable_enemy_to_throw(global_data);
            if (this->chosen_enemy != nullptr) {
                Boss_Node* boss = cast_boss_target(target);
                Enemy_Behavior_Component* enemy_bihavior = get_enemy_bihavior_component(this->chosen_enemy);
                enemy_bihavior->set_hooked_by_boss_skill(true);
                enemy_bihavior->set_boss_hooked_id(boss->get_enemy_id());
            }
            this->activating_skill(global_data);
        }
        Skill_Component::handle_task(target, delta_time, global_data);
    }

    void Boss_Skill_Throw_Enemy_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        this->boss_position = target->get_position();

        Character_Node* character = data->get_character_node();
        if (character == nullptr) {
            Skill_Component::update_information(target, delta_time, global_data);
            return;
        }
        this->character_position = character->get_position();

        Skill_Component::update_information(target, delta_time, global_data);
    }

    Enemy_Node* Boss_Skill_Throw_Enemy_Component::get_chosen_enemy() {
        return this->chosen_enemy;
    }

    float Boss_Skill_Throw_Enemy_Component::get_hook_enemy_direction() {
        if (this->chosen_enemy == nullptr) {
            return 0.f;
        }
        glm::vec2 enemy_position = this->chosen_enemy->get_position();
        return enemy_position.x > this->boss_position.x ? -1.f : 1.f;
    }

    float Boss_Skill_Throw_Enemy_Component::get_throwing_direction() {
        return this->character_position.x > this->boss_position.x ? -1.f : 1.f;
    }

    glm::vec2 Boss_Skill_Throw_Enemy_Component::get_boss_position() {
        return this->boss_position;
    }

    glm::vec2 Boss_Skill_Throw_Enemy_Component::get_end_throwing_position() {
        float distance = this->delta_end_position;
        glm::vec2 delta_position =
            glm::vec2(Math::random_float(-distance / 2, distance / 2), Math::random_float(-distance / 2, distance / 2));
        return this->character_position + delta_position;
    }

    Enemy_Node* Boss_Skill_Throw_Enemy_Component::find_suitable_enemy_to_throw(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Battle_Layer* battle_layer = data->get_battle_layer();
        if (battle_layer == nullptr) {
            return nullptr;
        }

        const Boss_Skill_Throw_Enemy_Config& skill_config = data->get_config().get_boss_skill_throw_enemy_config();
        const std::vector<Enemy_Node*>& enemies = battle_layer->get_enemies();
        bool has_suitable_enemy = false;
        for (Enemy_Node* enemy : enemies) {
            Enemy_State_Machine_Component* enemy_state_machine = get_enemy_state_machine_component(enemy);
            if (enemy_state_machine->is_enemy_dead() || enemy_state_machine->is_enemy_lost_all_health()) {
                continue;
            }
            glm::vec2 enemy_position = enemy->get_position();
            float distance = glm::distance(this->boss_position, enemy_position);
            if (distance <= skill_config.take_enemy_distance) {
                return enemy;
            }
        }

        return nullptr;
    }

    bool Boss_Skill_Throw_Enemy_Component::can_activate_skill(
        State_Machine_Component* state_machine, void* global_data
    ) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Boss_Skill_Throw_Enemy_Config& skill_config = data->get_config().get_boss_skill_throw_enemy_config();

        if (glm::distance(this->boss_position, this->character_position) > skill_config.activing_distance) {
            return false;
        }

        Character_Node* character = data->get_character_node();
        if (character == nullptr) {
            return false;
        }
        Character_State_Machine_Component* character_state_machine = get_character_state_machine(character);
        if (character_state_machine == nullptr) {
            return false;
        }
        if (character_state_machine->is_character_dead() || character_state_machine->is_character_dashing() ||
            character_state_machine->is_character_attacked() || character_state_machine->is_character_flight() ||
            character_state_machine->is_character_invincible()) {
            return false;
        }

        Boss_State_Machine_Component* boss_state_machine =
            reinterpret_cast<Boss_State_Machine_Component*>(state_machine);
        if (boss_state_machine->is_enemy_attacked() || boss_state_machine->is_enemy_attacked() ||
            boss_state_machine->is_enemy_dead() || boss_state_machine->is_using_some_skill(global_data)) {
            return false;
        }

        if (this->find_suitable_enemy_to_throw(global_data) == nullptr) {
            return false;
        }

        return Skill_Component::can_activate_skill(state_machine, global_data);
    }

    void Boss_Skill_Throw_Enemy_Component::activating_skill(void* global_data) {
        Skill_Component::activating_skill(global_data);
    }
}  // namespace Meow_Meow