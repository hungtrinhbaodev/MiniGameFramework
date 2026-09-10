#pragma once
#include <state_machine_component.h>

namespace Meow_Meow {
    class Character_State_Machine_Component : public State_Machine_Component {
    public:
        Character_State_Machine_Component();
        ~Character_State_Machine_Component();

        bool is_character_attacking();
        bool is_character_attacked();
        bool is_character_channelling();
        bool is_character_dead();
        bool is_character_stunned();
        bool is_character_dashing();
        bool is_character_moving();
        bool is_character_flight();
        bool is_character_invincible();
        bool can_character_attack(void* global_data);
        bool can_process_move_input(void* global_data);
        bool can_process_release_move(void* global_data);

    protected:
        void handle_auto_change_state(
            Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
        ) override;
    };
}  // namespace Meow_Meow