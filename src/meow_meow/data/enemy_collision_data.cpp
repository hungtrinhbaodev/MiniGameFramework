#include <meow_meow/data/enemy_collision_data.h>

namespace Meow_Meow {
    Enemy_Collision_Data::Enemy_Collision_Data() {}

    Enemy_Collision_Data::~Enemy_Collision_Data() {}

    void Enemy_Collision_Data::set_damage_deal(float damge_deal) {
        this->damage_deal = damge_deal;
    }

    void Enemy_Collision_Data::set_enemy_direction(glm::vec2 enemy_direction) {
        this->enemy_direction = enemy_direction;
    }

    float Enemy_Collision_Data::get_damage_deal() {
        return this->damage_deal;
    }

    glm::vec2 Enemy_Collision_Data::get_enemy_direction() {
        return this->enemy_direction;
    }

    std::string Enemy_Collision_Data::get_using_skill_id() {
        return this->using_skill;
    }

    float Enemy_Collision_Data::get_skill_damage() {
        return this->skill_damage;
    }

    void Enemy_Collision_Data::set_using_skill_id(std::string using_skill) {
        this->using_skill = using_skill;
    }

    void Enemy_Collision_Data::set_skill_damage(float skill_damage) {
        this->skill_damage = skill_damage;
    }
}  // namespace Meow_Meow