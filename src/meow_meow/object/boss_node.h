#pragma once
#include <meow_meow/object/enemy_node.h>

namespace Meow_Meow {
    class Boss_Node : public Enemy_Node {
    public:
        Boss_Node();
        Boss_Node(int enemy_id, int enemy_character_id);
        ~Boss_Node();

        const Enemy_Behavior_Config& get_behavior_config_from(void* global_data) const override;
        Enemy_Data& get_enemy_data(void* global_data) override;

    protected:
        void attach(void* global_data) override;
        Enemy_State_Machine_Component* make_state_machine_instance() override;
        Enemy_Behavior_Component* make_behavior_instance() override;
        void update_collision_component(Collision_Component* collision) override;
        void update_state_machine_component(State_Machine_Component* state_machine) override;
        void init_skill_components() override;
        void update_ui_attrubutes() override;
        void remove_from_battle(void* global_data) override;
        void clean_collision_data(Collision_Component* collision) override;
        Custom::Anchor_Point get_origin_animation_anchor_point() override;

    private:
        const int ACTION_CHANNELLING_SKILL_TAG = 0;
        const int ACTION_FLASHING_SKILL_TAG = 1;
        const int ACTION_HOOK_ENEMY_TAG = 2;
        const int ACTION_START_THROWING_ENEMY_TAG = 3;

        const Custom::Anchor_Point ORIGIN_ANIMATION_ANCHOR_POINT = {0.45, 0.35};
        const glm::vec2 ORIGIN_HEALTH_BAR_POSITION = {0, 130};

        void start_channelling(void* global_data, int source_call_state);
        void start_flashing(void* global_data);

        void end_channelling(void* global_data, int source_call_state);
        void end_flashing(void* global_data);

        void action_channelling_skill_flash(float delay, float duration_channelling);
        void action_flashing(float delay, float duration_float, glm::vec2 flash_position);
        void action_hook_enemy_to_throw(float delay, float duration_hook, float direction);
        void action_start_throw_enemy(float delay, float duration_start_throw, float direction);

        std::string channelling_skill = "";
    };
}  // namespace Meow_Meow