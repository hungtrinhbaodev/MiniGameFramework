#pragma once
#include <collision_component.h>

namespace Meow_Meow {
    class Character_Collision_Component : public Collision_Component {
    public:
        Character_Collision_Component();
        ~Character_Collision_Component();

        float get_damage_deal();
        glm::vec2 get_enemy_direction();

    protected:
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;

    private:
        float damage_deal = 0.f;
        glm::vec2 enemy_direction = {0.f, 0.f};
    };
}  // namespace Meow_Meow