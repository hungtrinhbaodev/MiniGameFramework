#include <math_custom.h>
#include <meow_meow/component/character_skill_thunder_component.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/data/global_data.h>
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

        this->attacking = false;
        this->walking = false;
        this->hitted_by_thunder_skill = false;

        if (character == nullptr) {
            this->enemy_direction = {0.f, 0.f};
            return;
        }

        State_Machine_Component* state_machine =
            Utils::get_component<State_Machine_Component>(character, Defined::COMPONENT_STATE_MACHINE_NAME);

        if (state_machine != nullptr &&
            (state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_DEATH ||
             state_machine->get_current_state_at(Const::TRACK_CONTROLL) == Const::STATE_FLIGHT)) {
            this->walking = true;
            this->enemy_direction = {0.f, 0.f};
            return;
        }

        Character_Skill_Thurnder_Component* skill_thunder = Utils::get_component<Character_Skill_Thurnder_Component>(
            character, Const::CHARACTER_SKILL_THUNDER_COMPONENT_NAME
        );

        if (skill_thunder != nullptr) {
            Enemy_Node* enemy = reinterpret_cast<Enemy_Node*>(target);
            if (skill_thunder->is_enemy_taken(enemy->get_enemy_id())) {
                this->hitted_by_thunder_skill = true;
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
    }

}  // namespace Meow_Meow