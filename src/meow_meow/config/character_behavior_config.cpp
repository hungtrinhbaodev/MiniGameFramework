#include <meow_meow/config/character_behavior_config.h>

namespace Meow_Meow {
    Character_Behavior_Config::Character_Behavior_Config() {}

    Character_Behavior_Config::Character_Behavior_Config(
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
    ) {
        this->attacked_duration = attacked_duration;
        this->invincible_duration = invincible_duration;
        this->velosity = velosity;
        this->accelarate = accelarate;
        this->attack_duration = attack_duration;
        this->bonus_velosity_rate = bonus_velosity_rate;
        this->bonus_velosity = bonus_velosity;
        this->max_bonus_velosity = max_bonus_velosity;
        this->bounding_box = bounding_box;
        this->max_health = max_health;
        this->damage = damage;
        this->dead_duration = dead_duration;
    }

    Character_Behavior_Config::~Character_Behavior_Config() {}

    float Character_Behavior_Config::get_attacked_duration() const {
        return this->attacked_duration;
    }

    float Character_Behavior_Config::get_invincible_duration() const {
        return this->invincible_duration;
    }

    float Character_Behavior_Config::get_velosity() const {
        return this->velosity;
    }

    float Character_Behavior_Config::get_accelarate() const {
        return this->accelarate;
    }

    float Character_Behavior_Config::get_attack_duration() const {
        return this->attack_duration;
    }

    float Character_Behavior_Config::get_bonus_velosity_rate() const {
        return this->bonus_velosity_rate;
    }

    float Character_Behavior_Config::get_bonus_velosity() const {
        return this->bonus_velosity;
    }

    float Character_Behavior_Config::get_max_bonus_velosity() const {
        return this->max_bonus_velosity;
    }

    Custom::Size Character_Behavior_Config::get_bounding_box() const {
        return this->bounding_box;
    }

    float Character_Behavior_Config::get_max_health() const {
        return this->max_health;
    }

    float Character_Behavior_Config::get_damage() const {
        return this->damage;
    }

    float Character_Behavior_Config::get_dead_duration() const {
        return this->dead_duration;
    }

}  // namespace Meow_Meow