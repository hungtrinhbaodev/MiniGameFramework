#pragma once
#include <base_component.h>
#include <meow_meow/config/enemy_behavior_config.h>

namespace Meow_Meow {
    class Enemy_Behavior_Component : public Base_Component {
    public:
        Enemy_Behavior_Component();
        ~Enemy_Behavior_Component();

        void start_attack_countdown();

        bool is_walking();
        bool can_attack();
        bool is_hitted_by_thunder_skill();

        glm::vec2 get_enemy_walking_direction();

    protected:
        void update_information(Base_Node* target, float delta_time, void* global_data) override;
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;
        virtual const Enemy_Behavior_Config& get_behavior_config(void* global_data) const;

    private:
        bool walking = false;
        bool attacking = false;
        bool hitted_by_thunder_skill = false;

        bool is_attack_countdown = false;
        float attack_countdown = 0;

        glm::vec2 enemy_direction{0.f, 0.f};
        glm::vec2 jump_position{0.f, 0.f};
    };
}  // namespace Meow_Meow