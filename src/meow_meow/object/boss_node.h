#pragma once
#include <meow_meow/object/enemy_node.h>

namespace Meow_Meow {
    class Boss_Node : public Enemy_Node {
    public:
        Boss_Node();
        Boss_Node(int enemy_id, int enemy_character_id);
        ~Boss_Node();

    protected:
        const Enemy_Behavior_Config& get_behavior_config_from(void* global_data) const override;
        void update_collision_component(Collision_Component* collision) override;
        void init_skill_components() override;
        bool handle_active_skill(void* global_data) override;
        bool handle_other_state(void* global_data) override;
        void remove_from_battle(void* global_data) override;
        void clean_collision_data(Collision_Component* collision) override;

    private:
        const int ACTION_CHANNELLING_SKILL_TAG = 0;
        const int ACTION_FLASHING_SKILL_TAG = 1;

        void change_to_channeling(void* global_data, std::string skill_id);
        void change_to_flash(void* global_data);

        void action_channelling_skill(float delay, float duration_channelling);
        void action_flashing(float delay, float duration_float, glm::vec2 flash_position);

        std::string channelling_skill = "";
    };
}  // namespace Meow_Meow