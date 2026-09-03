#pragma once
#include <meow_meow/component/character_skill_component.h>

#include <vector>

namespace Meow_Meow {
    class Character_Skill_Thurnder_Component : public Character_Skill_Component {
    public:
        Character_Skill_Thurnder_Component();
        ~Character_Skill_Thurnder_Component();

        bool is_enemy_taken(int enemy_id);

        bool can_activate_skill(State_Machine_Component* state_machine, void* global_data) override;
        void activating_skill(void* global_data) override;

    protected:
        void attach(Base_Node* target, void* global_data) override;
        void update_information(Base_Node* target, float delta_time, void* global_data) override;

    private:
        std::vector<int> enemy_taken_ids;
    };
}  // namespace Meow_Meow