#pragma once
#include <string>

namespace Meow_Meow {
    struct Boss_Skill_Flash_Config {
        std::string skill_id = "";
        float duration_countdown = 0.f;
        float duration_flash = 0.f;
        float duration_channeling = 0.f;
        float flash_damage = 0.f;
        float flash_distance = 0.f;
        float fly_duration = 0.f;
        float stun_duration = 0.f;
    };
}  // namespace Meow_Meow