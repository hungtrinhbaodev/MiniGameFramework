#include <meow_meow/data/bullet_collision_data.h>

namespace Meow_Meow {
    Bullet_Collision_Data::Bullet_Collision_Data() {}

    Bullet_Collision_Data::~Bullet_Collision_Data() {}

    void Bullet_Collision_Data::set_damage_deal(float damage_deal) {
        this->damage_deal = damage_deal;
    }

    void Bullet_Collision_Data::set_bullet_direction(Const::DIRECTION bullet_direction) {
        this->bullet_direction = bullet_direction;
    }

    float Bullet_Collision_Data::get_damage_deal() {
        return this->damage_deal;
    }

    Const::DIRECTION Bullet_Collision_Data::get_bullet_direction() {
        return this->bullet_direction;
    }
}  // namespace Meow_Meow