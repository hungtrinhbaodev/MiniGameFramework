#pragma once
#include <meow_meow/component/skill_component.h>
#include <meow_meow/object/enemy_node.h>

namespace Meow_Meow {
    class Boss_Skill_Throw_Enemy_Component : public Skill_Component {
    public:
        Boss_Skill_Throw_Enemy_Component();
        ~Boss_Skill_Throw_Enemy_Component();

        Enemy_Node* get_chosen_enemy();
        float get_hook_enemy_direction();
        float get_throwing_direction();
        glm::vec2 get_boss_position();
        glm::vec2 get_end_throwing_position();

        bool can_activate_skill(State_Machine_Component* state_machine, void* global_data) override;
        void activating_skill(void* global_data) override;

    protected:
        void attach(Base_Node* target, void* global_data) override;
        void handle_task(Base_Node* target, float delta_time, void* global_data) override;
        void update_information(Base_Node* target, float delta_time, void* global_data) override;

    private:
        glm::vec2 boss_position = {0.f, 0.f};
        glm::vec2 character_position = {0.f, 0.f};
        Enemy_Node* chosen_enemy = nullptr;
        float delta_end_position = 0.f;

        Enemy_Node* find_suitable_enemy_to_throw(void* global_data);
    };
}  // namespace Meow_Meow