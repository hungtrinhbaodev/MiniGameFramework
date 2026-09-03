#pragma once
#include <string>

namespace Meow_Meow {
    struct Character_Skill_Thunder_Config {
        std::string skill_id = "";
        float duration_countdown = 0.f;
        int number_taken = 0;
        float channelling_duration = 0.f;
        float radius_taken = 0.f;
        float damage_taken = 0.f;
        float stun_duration = 0.f;
    };
}  // namespace Meow_Meow