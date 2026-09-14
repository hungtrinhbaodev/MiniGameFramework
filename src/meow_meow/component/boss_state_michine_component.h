#pragma once
#include <meow_meow/component/enemy_state_machine_component.h>

namespace Meow_Meow {
    class Boss_State_Machine_Component : public Enemy_State_Machine_Component {
    public:
        Boss_State_Machine_Component();
        ~Boss_State_Machine_Component();

        bool can_take_bullet_damage(void* global_data) override;
        bool is_channelling_skill_flash(void* global_data);
        bool is_channelling_skill_throw_enemy(void* global_data);
        bool is_using_skill_falsh(void* global_data);
        bool is_using_skill_throw_enemy(void* global_data);
        bool is_using_some_skill(void* global_data);

    protected:
        void handle_auto_change_state(
            Base_Node* target, std::string track, std::string state, void* global_data, int source_call_state
        ) override;
    };
}  // namespace Meow_Meow