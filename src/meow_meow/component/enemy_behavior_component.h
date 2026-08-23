#pragma once
#include <base_component.h>
#include <meow_meow/config/enemy_behavior_config.h>

namespace Meow_Meow {
    class Enemy_Behavior_Component : public Base_Component {
    public:
        Enemy_Behavior_Component();
        ~Enemy_Behavior_Component();

        void start_attack_countdown();
        void start_jump_countdown();

        bool is_walking();
        bool can_jump();
        bool can_attack();

        glm::vec2 get_enemy_walking_direction();
        glm::vec2 get_jump_position();

    protected:
        void update_information(Base_Node* target, float delta_time, void* global_data) override;
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;

    private:
        bool walking = false;
        bool jumping = false;
        bool attacking = false;

        bool is_attack_countdown = false;
        bool is_jump_countdown = false;
        float attack_countdown = 0;
        float jump_countdown = 0;

        glm::vec2 enemy_direction{0.f, 0.f};
        glm::vec2 jump_position{0.f, 0.f};
    };
}  // namespace Meow_Meow