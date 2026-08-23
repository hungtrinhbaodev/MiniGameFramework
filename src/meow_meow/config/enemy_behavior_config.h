#pragma once

namespace Meow_Meow {
    struct Enemy_Behavior_Config {
        float enemy_jump_distance = 0.f;
        float enemy_velosity = 0.f;
        float enemy_attack_damge = 0.f;
        float enemy_attack_range = 0.f;
        float enemy_walk_duration = 0.f;
        float enemy_jump_duration = 0.f;
        float enemy_attack_duration = 0.f;
        float enemy_attack_countdown = 0.f;
        float enemy_jump_countdown = 0.f;

        float get_enemy_jump_distane() const;
        float get_enemy_velosity() const;
        float get_emeny_attack_damge() const;
        float get_emeny_attack_ranage() const;
        float get_enemy_walk_duration() const;
        float get_enemy_jump_duration() const;
        float get_enemy_attack_duration() const;
        float get_enemy_attack_countdown() const;
        float get_enemy_jump_countdown() const;
    };
}  // namespace Meow_Meow