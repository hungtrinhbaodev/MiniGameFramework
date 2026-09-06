#pragma once
#include <meow_meow/component/skill_component.h>

namespace Meow_Meow {
    class Character_Skill_Dash_Component : public Skill_Component {
    public:
        Character_Skill_Dash_Component();
        ~Character_Skill_Dash_Component();

        bool can_activate_skill(State_Machine_Component* state_machine, void* global_data) override;

    protected:
        void attach(Base_Node* target, void* global_data) override;
    };
}  // namespace Meow_Meow