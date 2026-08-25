#pragma once

namespace Meow_Meow {
    class Character_Behavior_Config {
    public:
        Character_Behavior_Config();
        Character_Behavior_Config(float attacked_duration, float invincible_duration);
        ~Character_Behavior_Config();

        float get_attacked_duration() const;
        float get_invincible_duration() const;

    private:
        float attacked_duration = 0.f;
        float invincible_duration = 0.f;
    };
}  // namespace Meow_Meow