#pragma once
#include <base_component.h>
#include <state_machine_component.h>

#include <string>

namespace Meow_Meow {
    class Character_Skill_Component : public Base_Component {
    public:
        Character_Skill_Component();
        ~Character_Skill_Component();

        virtual bool can_activate_skill(State_Machine_Component* state_machine, void* global_data);
        virtual void activating_skill(void* global_data);

    protected:
        void update_information(Base_Node* target, float delta_time, void* global_data) override;
        void set_skill_id(std::string skill_id);
        void set_countdown_time(float countdown_time);

    private:
        std::string skill_id = "";
        float countdown_time = 0.f;
        float current_countdown_time = 0.f;

        void start_countdown_time();
    };
}  // namespace Meow_Meow