#pragma once
#include <meow_meow/component/skill_component.h>

namespace Meow_Meow {
    class Enemy_Jump_Skill_Component : public Skill_Component {
    public:
        Enemy_Jump_Skill_Component();
        ~Enemy_Jump_Skill_Component();

        bool can_activate_skill(State_Machine_Component* state_machine, void* global_data) override;
        void activating_skill(void* global_data) override;

        glm::vec2 get_jump_position();

    protected:
        void attach(Base_Node* target, void* global_data) override;
        void update_information(Base_Node* target, float delta_time, void* global_data) override;

    private:
        glm::vec2 player_position = {0.f, 0.f};
        glm::vec2 enemy_position = {0.f, 0.f};
        glm::vec2 jump_position = {0.f, 0.f};
    };
}  // namespace Meow_Meow