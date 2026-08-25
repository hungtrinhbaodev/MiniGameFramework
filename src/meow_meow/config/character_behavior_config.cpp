#include <meow_meow/config/character_behavior_config.h>

namespace Meow_Meow {
    Character_Behavior_Config::Character_Behavior_Config() {}

    Character_Behavior_Config::Character_Behavior_Config(float attacked_duration, float invincible_duration) {
        this->attacked_duration = attacked_duration;
        this->invincible_duration = invincible_duration;
    }

    Character_Behavior_Config::~Character_Behavior_Config() {}

    float Character_Behavior_Config::get_attacked_duration() const {
        return this->attacked_duration;
    }

    float Character_Behavior_Config::get_invincible_duration() const {
        return this->invincible_duration;
    }
}  // namespace Meow_Meow