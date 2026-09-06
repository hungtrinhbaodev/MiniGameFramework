#pragma once
#include <string>

namespace Meow_Meow {
    struct Enemy_Skill_Jump_Config {
        std::string skill_id = "";
        float duration_countdown = 0.f;
        float jump_distance = 0.f;
        float duration_jump = 0.f;
    };
}  // namespace Meow_Meow