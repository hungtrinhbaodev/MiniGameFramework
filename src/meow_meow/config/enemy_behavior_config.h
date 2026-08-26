#pragma once
#include <custom.h>

namespace Meow_Meow {
    class Enemy_Behavior_Config {
    public:
        Enemy_Behavior_Config();
        Enemy_Behavior_Config(
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
        );
        ~Enemy_Behavior_Config();

        float get_enemy_jump_distane() const;
        float get_enemy_velosity() const;
        float get_emeny_attack_damage() const;
        float get_emeny_attack_ranage() const;
        float get_enemy_walk_duration() const;
        float get_enemy_jump_duration() const;
        float get_enemy_attack_duration() const;
        float get_enemy_attack_countdown() const;
        float get_enemy_jump_countdown() const;
        float get_enemy_attacked_duration() const;
        float get_enemy_attacked_velosity() const;
        float get_enemy_health() const;
        Custom::Size get_bounding_box() const;

    private:
        float enemy_jump_distance = 0.f;
        float enemy_velosity = 0.f;
        float enemy_attack_damage = 0.f;
        float enemy_attack_range = 0.f;
        float enemy_walk_duration = 0.f;
        float enemy_jump_duration = 0.f;
        float enemy_attack_duration = 0.f;
        float enemy_attack_countdown = 0.f;
        float enemy_jump_countdown = 0.f;
        float enemy_attacked_duration = 0.f;
        float enemy_attacked_velosity = 0.f;
        float enemy_health = 0.f;
        Custom::Size bounding_box = {0.f, 0.f};
    };
}  // namespace Meow_Meow