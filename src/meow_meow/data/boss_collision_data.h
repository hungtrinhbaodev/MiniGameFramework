#pragma once
#include <meow_meow/data/enemy_collision_data.h>

namespace Meow_Meow {
    class Boss_Collision_Data : public Enemy_Collision_Data {
    public:
        Boss_Collision_Data();
        ~Boss_Collision_Data();

        std::string get_using_skill_id();
        float get_skill_damage();

        void set_using_skill_id(std::string using_skill);
        void set_skill_damage(float skill_damage);

    private:
        std::string using_skill = "";
        float skill_damage = 0.f;
    };
}  // namespace Meow_Meow