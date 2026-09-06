#pragma once
#include <meow_meow/component/skill_component.h>

namespace Meow_Meow {
    class Boss_Skill_Flash_Component : public Skill_Component {
    public:
        Boss_Skill_Flash_Component();
        ~Boss_Skill_Flash_Component();

        bool can_activate_skill(State_Machine_Component* state_machine, void* global_data) override;
        void activating_skill(void* global_data) override;
        glm::vec2 get_flash_to_position();

    protected:
        void attach(Base_Node* target, void* global_data) override;
        void update_information(Base_Node* target, float delta_time, void* global_data) override;

    private:
        glm::vec2 player_position = {0.f, 0.f};
        glm::vec2 boss_position = {0.f, 0.f};
        float boss_attack_distance = 0.f;
    };
}  // namespace Meow_Meow
