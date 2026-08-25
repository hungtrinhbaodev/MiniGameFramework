#pragma once
#include <custom.h>

namespace Meow_Meow {
    class Bullet_Behavior_Config {
    public:
        Bullet_Behavior_Config();
        Bullet_Behavior_Config(
            float bullet_damage, float bullet_velosity, float bullet_accelarate, Custom::Size bullet_bounding_box
        );
        ~Bullet_Behavior_Config();

        float get_bullet_damage() const;
        float get_bullet_velosity() const;
        float get_bullet_accelarate() const;
        Custom::Size get_bullet_bounding_box() const;

    private:
        float bullet_damage = 0.f;
        float bullet_velosity = 0.f;
        float bullet_accelarate = 0.f;
        Custom::Size bullet_bounding_box = {0.f, 0.f};
    };
}  // namespace Meow_Meow