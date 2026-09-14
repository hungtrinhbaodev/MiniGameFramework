#pragma once
#include <string>

namespace Meow_Meow {
    struct Boss_Skill_Throw_Enemy_Config {
        std::string skill_id = "";
        float duration_countdown = 0.f;
        float duration_channelling = 0.f;
        float duration_throwing = 0.f;
        float duration_stun = 0.f;
        float skill_damage = 0.f;
        float pop_up_distance = 0.f;
        float take_enemy_distance = 0.f;
        float activing_distance = 0.f;
        float random_delta_end_range = 0.f;
        float duration_hook_enemy = 0.f;
        float duration_start_throwing_enemy = 0.f;
        float shake_distance = 0.f;
        float distance_hit_collision = 0.f;
    };
}  // namespace Meow_Meow