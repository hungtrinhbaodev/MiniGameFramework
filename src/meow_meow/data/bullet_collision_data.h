#pragma once
#include <meow_meow/const.h>

#include <glm/glm.hpp>

namespace Meow_Meow {
    class Bullet_Collision_Data {
    public:
        Bullet_Collision_Data();
        ~Bullet_Collision_Data();

        float get_damage_deal();
        Const::DIRECTION get_bullet_direction();

        void set_damage_deal(float damage_deal);
        void set_bullet_direction(Const::DIRECTION direction);

    private:
        float damage_deal = 0.f;
        Const::DIRECTION bullet_direction = Const::DIRECTION::LEFT;
    };
}  // namespace Meow_Meow