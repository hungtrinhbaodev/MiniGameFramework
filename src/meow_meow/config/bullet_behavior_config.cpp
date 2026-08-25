#include <meow_meow/config/bullet_behavior_config.h>

namespace Meow_Meow {
    Bullet_Behavior_Config::Bullet_Behavior_Config() {}

    Bullet_Behavior_Config::Bullet_Behavior_Config(
        float bullet_damage, float bullet_velosity, float bullet_accelarate, Custom::Size bullet_bounding_box
    ) {
        this->bullet_damage = bullet_damage;
        this->bullet_velosity = bullet_velosity;
        this->bullet_accelarate = bullet_accelarate;
        this->bullet_bounding_box = bullet_bounding_box;
    }

    Bullet_Behavior_Config::~Bullet_Behavior_Config() {}

    float Bullet_Behavior_Config::get_bullet_damage() const {
        return this->bullet_damage;
    }

    float Bullet_Behavior_Config::get_bullet_velosity() const {
        return this->bullet_velosity;
    }

    float Bullet_Behavior_Config::get_bullet_accelarate() const {
        return this->bullet_accelarate;
    }

    Custom::Size Bullet_Behavior_Config::get_bullet_bounding_box() const {
        return this->bullet_bounding_box;
    }

};  // namespace Meow_Meow