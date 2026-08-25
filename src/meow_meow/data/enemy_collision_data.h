#pragma once
#include <glm/glm.hpp>

namespace Meow_Meow {
    class Enemy_Collision_Data {
    public:
        Enemy_Collision_Data();
        ~Enemy_Collision_Data();

        float get_damage_deal();
        glm::vec2 get_enemy_direction();

        void set_damage_deal(float damge_deal);
        void set_enemy_direction(glm::vec2 enemy_direction);

    private:
        float damage_deal = 0;
        glm::vec2 enemy_direction = {0.f, 0.f};
    };
}  // namespace Meow_Meow