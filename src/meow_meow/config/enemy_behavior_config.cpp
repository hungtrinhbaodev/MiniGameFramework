#include <meow_meow/config/enemy_behavior_config.h>

namespace Meow_Meow {

    Enemy_Behavior_Config::Enemy_Behavior_Config() {}

    Enemy_Behavior_Config::Enemy_Behavior_Config(
        float enemy_jump_distance,
        float enemy_velosity,
        float enemy_attack_damage,
        float enemy_attack_range,
        float enemy_walk_duration,
        float enemy_jump_duration,
        float enemy_attack_duration,
        float enemy_attack_countdown,
        float enemy_jump_countdown,
        float enemy_attacked_duration,
        float enemy_attacked_velosity,
        float enemy_health,
        Custom::Size bounding_box
    ) {
        this->enemy_jump_distance = enemy_jump_distance;
        this->enemy_velosity = enemy_velosity;
        this->enemy_attack_damage = enemy_attack_damage;
        this->enemy_attack_range = enemy_attack_range;
        this->enemy_walk_duration = enemy_walk_duration;
        this->enemy_jump_duration = enemy_jump_duration;
        this->enemy_attack_duration = enemy_attack_duration;
        this->enemy_attack_countdown = enemy_attack_countdown;
        this->enemy_jump_countdown = enemy_jump_countdown;
        this->enemy_attacked_duration = enemy_attacked_duration;
        this->enemy_attacked_velosity = enemy_attacked_velosity;
        this->enemy_health = enemy_health;
        this->bounding_box = bounding_box;
    }

    Enemy_Behavior_Config::~Enemy_Behavior_Config() {}

    float Enemy_Behavior_Config::get_enemy_jump_distane() const {
        return this->enemy_jump_distance;
    }

    float Enemy_Behavior_Config::get_enemy_velosity() const {
        return this->enemy_velosity;
    }

    float Enemy_Behavior_Config::get_emeny_attack_damage() const {
        return this->enemy_attack_damage;
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

    float Enemy_Behavior_Config::get_enemy_attacked_duration() const {
        return this->enemy_attacked_duration;
    }

    float Enemy_Behavior_Config::get_enemy_attacked_velosity() const {
        return this->enemy_attacked_velosity;
    }

    float Enemy_Behavior_Config::get_enemy_health() const {
        return this->enemy_health;
    }

    Custom::Size Enemy_Behavior_Config::get_bounding_box() const {
        return this->bounding_box;
    }

}  // namespace Meow_Meow