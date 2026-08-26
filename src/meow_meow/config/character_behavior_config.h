#pragma once
#include <custom.h>

namespace Meow_Meow {
    class Character_Behavior_Config {
    public:
        Character_Behavior_Config();
        Character_Behavior_Config(
            float attacked_duration,
            float invincible_duration,
            float velosity,
            float accelarate,
            float attack_duration,
            float bonus_velosity_rate,
            float bonus_velosity,
            float max_bonus_velosity,
            Custom::Size bounding_box,
            float max_health,
            float damage,
            float dead_duration
        );
        ~Character_Behavior_Config();

        float get_attacked_duration() const;
        float get_invincible_duration() const;
        float get_velosity() const;
        float get_accelarate() const;
        float get_attack_duration() const;
        float get_bonus_velosity_rate() const;
        float get_bonus_velosity() const;
        float get_max_bonus_velosity() const;
        Custom::Size get_bounding_box() const;
        float get_max_health() const;
        float get_damage() const;
        float get_dead_duration() const;

    private:
        float attacked_duration = 0.f;
        float invincible_duration = 0.f;
        float velosity = 0.f;
        float accelarate = 0.f;
        float attack_duration = 0.f;
        float bonus_velosity_rate = 0.f;
        float bonus_velosity = 0.f;
        float max_bonus_velosity = 0.f;
        Custom::Size bounding_box = {0.f, 0.f};
        float max_health = 0.f;
        float damage = 0.f;
        float dead_duration;
    };
}  // namespace Meow_Meow