#include <math_custom.h>
#include <meow_meow/component/enemy_behavior_component.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/global_data.h>

namespace Meow_Meow {
    Enemy_Behavior_Component::Enemy_Behavior_Component() {}

    Enemy_Behavior_Component::~Enemy_Behavior_Component() {}

    void Enemy_Behavior_Component::start_attack_countdown() {
        this->is_attack_countdown = true;
        this->attack_countdown = 0;
    }

    void Enemy_Behavior_Component::start_jump_countdown() {
        this->is_jump_countdown = true;
        this->jump_countdown = 0;
    }

    bool Enemy_Behavior_Component::is_walking() {
        return this->walking;
    }

    bool Enemy_Behavior_Component::can_jump() {
        return this->jumping;
    }

    bool Enemy_Behavior_Component::can_attack() {
        return this->attacking;
    }

    glm::vec2 Enemy_Behavior_Component::get_enemy_walking_direction() {
        return this->enemy_direction;
    }

    glm::vec2 Enemy_Behavior_Component::get_jump_position() {
        return this->jump_position;
    }

    void Enemy_Behavior_Component::handle_task(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Enemy_Behavior_Config behavior_config = data->get_config().get_enemy_behavior_config();
        Character_Node* character = data->get_character();

        glm::vec2 enemy_position = target->get_position();
        glm::vec2 character_position = character->get_position();

        this->attacking = false;
        this->jumping = false;
        this->walking = false;

        if (glm::distance(enemy_position, character_position) <= behavior_config.get_emeny_attack_ranage() &&
            !this->is_attack_countdown) {
            this->attacking = true;
            return;
        }

        if (glm::distance(enemy_position, character_position) <= behavior_config.get_enemy_jump_distane() &&
            !this->is_jump_countdown) {
            this->jumping = true;
            this->jump_position =
                character_position +
                glm::vec2{
                    Math::random_float(
                        -behavior_config.get_emeny_attack_ranage(), behavior_config.get_emeny_attack_ranage()
                    ),
                    Math::random_float(
                        -behavior_config.get_emeny_attack_ranage(), behavior_config.get_emeny_attack_ranage()
                    )
                };
            return;
        }

        this->walking = true;
        this->enemy_direction = glm::normalize(character_position - enemy_position);
    }

    void Enemy_Behavior_Component::update_information(Base_Node* target, float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Enemy_Behavior_Config behavior_config = data->get_config().get_enemy_behavior_config();

        if (this->is_attack_countdown) {
            this->attack_countdown += delta_time;
            if (this->attack_countdown >= behavior_config.get_enemy_attack_countdown()) {
                this->is_attack_countdown = false;
            }
        }

        if (this->is_jump_countdown) {
            this->jump_countdown += delta_time;
            if (this->jump_countdown >= behavior_config.get_enemy_jump_countdown()) {
                this->is_jump_countdown = false;
            }
        }
    }

}  // namespace Meow_Meow