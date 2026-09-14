#include <math_custom.h>
#include <meow_meow/component/character_skill_thunder_component.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/utils.h>
#include <utils.h>

namespace Meow_Meow {
    Enemy_Behavior_Component::Enemy_Behavior_Component() {}

    Enemy_Behavior_Component::~Enemy_Behavior_Component() {}

    void Enemy_Behavior_Component::start_attack_countdown() {
        this->is_attack_countdown = true;
        this->attack_countdown = 0;
    }

    bool Enemy_Behavior_Component::is_walking() {
        return this->walking;
    }

    bool Enemy_Behavior_Component::can_attack() {
        return this->attacking;
    }

    bool Enemy_Behavior_Component::is_hitted_by_thunder_skill() {
        return this->hitted_by_thunder_skill;
    }

    glm::vec2 Enemy_Behavior_Component::get_enemy_walking_direction() {
        return this->enemy_direction;
    }

    int Enemy_Behavior_Component::get_boss_hocked_id() {
        return this->boss_hooked_id;
    }

    void Enemy_Behavior_Component::set_hooked_by_boss_skill(bool hooked) {
        this->hooked_by_boss_skill = hooked;
    }

    void Enemy_Behavior_Component::set_boss_hooked_id(int boss_id) {
        this->boss_hooked_id = boss_id;
    }

    const Enemy_Behavior_Config& Enemy_Behavior_Component::get_behavior_config(void* global_data) const {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        return data->get_config().get_enemy_behavior_config();
    }

    void Enemy_Behavior_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Enemy_Behavior_Config& behavior_config = this->get_behavior_config(global_data);
        Character_Node* character = data->get_character_node();

        glm::vec2 enemy_position = target->get_position();
        glm::vec2 character_position = character->get_position();

        if (character == nullptr) {
            this->enemy_direction = {0.f, 0.f};
            return;
        }

        Enemy_State_Machine_Component* enemy_state_machine = get_enemy_state_machine_component(target);
        Character_State_Machine_Component* character_state_machine = get_character_state_machine(target);

        if (enemy_state_machine->is_enemy_dead() || enemy_state_machine->is_enemy_lost_all_health() ||
            enemy_state_machine->is_enemy_hooked()) {
            if (enemy_state_machine->is_enemy_hooked() && this->boss_hooked_id != -1) {
                Boss_Skill_Throw_Enemy_Component* throw_skill =
                    get_skill_throw_enemy_component(global_data, this->boss_hooked_id);
                if (throw_skill != nullptr) {
                    this->enemy_direction = {throw_skill->get_hook_enemy_direction(), 0};
                }
            }
            return;
        }

        if (this->hooked_by_boss_skill) {
            this->enemy_direction = {0.f, 0.f};
            const Boss_Skill_Throw_Enemy_Config& skill_config = data->get_config().get_boss_skill_throw_enemy_config();
            enemy_state_machine->change_state_at(
                Const::TRACK_CONTROLL,
                Const::STATE_SKILL_CHANNELLING,
                skill_config.duration_channelling,
                Const::ENEMY_CHANNLING_BY_SKILL_THROWING
            );
            this->hooked_by_boss_skill = false;
            return;
        }

        if (character_state_machine != nullptr &&
            (character_state_machine->is_character_dead() || character_state_machine->is_character_flight())) {
            this->enemy_direction = {0.f, 0.f};
            return;
        }

        Character_Skill_Thunder_Component* skill_thunder = Utils::get_component<Character_Skill_Thunder_Component>(
            character, Const::CHARACTER_SKILL_THUNDER_COMPONENT_NAME
        );

        if (skill_thunder != nullptr) {
            Enemy_Node* enemy = reinterpret_cast<Enemy_Node*>(target);
            if (skill_thunder->is_enemy_taken(enemy->get_enemy_id())) {
                const Character_Skill_Thunder_Config skill_config =
                    data->get_config().get_character_skill_thunder_config();
                enemy_state_machine->change_state_at(
                    Const::TRACK_EFFECTED,
                    Const::STATE_STUN,
                    skill_config.stun_duration,
                    Const::ENEMY_STUN_FROM_CHARACTER_SKILL_THUNDER
                );
                enemy_state_machine->change_state_at(
                    Const::TRACK_CONTROLL, Const::STATE_IDLE, State_Machine_Component::INFITY_STATE
                );
                return;
            }
        }

        if (glm::distance(enemy_position, character_position) <= behavior_config.get_emeny_attack_ranage() &&
            !this->is_attack_countdown) {
            this->attacking = true;
            return;
        }

        this->walking = true;
    }

    void Enemy_Behavior_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Enemy_Behavior_Config behavior_config = data->get_config().get_enemy_behavior_config();
        Character_Node* character = data->get_character_node();

        if (this->is_attack_countdown) {
            this->attack_countdown += delta_time;
            if (this->attack_countdown >= behavior_config.get_enemy_attack_countdown()) {
                this->is_attack_countdown = false;
            }
        }

        glm::vec2 enemy_position = target->get_position();
        glm::vec2 character_position = character->get_position();

        this->enemy_direction =
            glm::normalize(character_position - enemy_position + glm::vec2(0, Math::random_float(-200, 200)));

        this->attacking = false;
        this->walking = false;
        this->hitted_by_thunder_skill = false;
    }

}  // namespace Meow_Meow