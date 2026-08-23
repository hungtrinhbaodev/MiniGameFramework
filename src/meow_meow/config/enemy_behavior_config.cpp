#include <meow_meow/config/enemy_behavior_config.h>

namespace Meow_Meow {

    float Enemy_Behavior_Config::get_enemy_jump_distane() const {
        return this->enemy_jump_distance;
    }

    float Enemy_Behavior_Config::get_enemy_velosity() const {
        return this->enemy_velosity;
    }

    float Enemy_Behavior_Config::get_emeny_attack_damge() const {
        return this->enemy_attack_damge;
    }

    float Enemy_Behavior_Config::get_emeny_attack_ranage() const {
        return this->enemy_attack_range;
    }

    float Enemy_Behavior_Config::get_enemy_walk_duration() const {
        return this->enemy_walk_duration;
    }

    float Enemy_Behavior_Config::get_enemy_jump_duration() const {
        return this->enemy_jump_duration;
    }

    float Enemy_Behavior_Config::get_enemy_attack_duration() const {
        return this->enemy_attack_duration;
    }

    float Enemy_Behavior_Config::get_enemy_attack_countdown() const {
        return this->enemy_attack_countdown;
    }

    float Enemy_Behavior_Config::get_enemy_jump_countdown() const {
        return this->enemy_jump_countdown;
    }

}  // namespace Meow_Meow