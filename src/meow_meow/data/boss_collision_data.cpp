#include <meow_meow/data/boss_collision_data.h>

namespace Meow_Meow {
    Boss_Collision_Data::Boss_Collision_Data() {}

    Boss_Collision_Data::~Boss_Collision_Data() {}

    std::string Boss_Collision_Data::get_using_skill_id() {
        return this->using_skill;
    }

    float Boss_Collision_Data::get_skill_damage() {
        return this->skill_damage;
    }

    void Boss_Collision_Data::set_using_skill_id(std::string using_skill) {
        this->using_skill = using_skill;
    }

    void Boss_Collision_Data::set_skill_damage(float skill_damage) {
        this->skill_damage = skill_damage;
    }
}  // namespace Meow_Meow