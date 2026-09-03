#pragma once
#include <string>

namespace Meow_Meow {
    struct Character_Skill_Dash_Config {
        float duration_countdown = 0.f;
        std::string skill_id = "";
        float dash_duration = 0.f;
        float dash_distance = 0.f;
    };
}  // namespace Meow_Meow