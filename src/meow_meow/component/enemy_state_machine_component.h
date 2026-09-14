#pragma once
#include <state_machine_component.h>

namespace Meow_Meow {
    class Enemy_State_Machine_Component : public State_Machine_Component {
    public:
        Enemy_State_Machine_Component();
        ~Enemy_State_Machine_Component();

        virtual bool can_take_bullet_damage(void* global_data);
        bool is_enemy_dead();
        bool is_enemy_attacked();
        bool is_enemy_stun();
        bool is_enemy_jump();
        bool is_enemy_lost_all_health();
        bool is_enemy_hooked();

        void change_state_at(
            std::string track,
            std::string state,
            float state_duration,
            int source_call_tag = -1,
            bool is_calling_end_last_state = true
        ) override;

    protected:
        void handle_auto_change_state(
            Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
        ) override;
    };
}  // namespace Meow_Meow