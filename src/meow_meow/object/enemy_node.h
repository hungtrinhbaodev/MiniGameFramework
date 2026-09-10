#pragma once
#include <collision_component.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/component/enemy_state_machine_component.h>
#include <meow_meow/config/enemy_behavior_config.h>
#include <meow_meow/object/game_object.h>
#include <progression_node.h>
#include <state_machine_component.h>

namespace Meow_Meow {
    class Enemy_Node : public Game_Object {
    public:
        Enemy_Node();
        Enemy_Node(int enemy_id, int enemy_character_id);
        ~Enemy_Node();

        int get_enemy_id();
        virtual const Enemy_Behavior_Config& get_behavior_config_from(void* global_data) const;
        virtual Enemy_Data& get_enemy_data(void* global_data);

    protected:
        Custom::Transformed_Rectangle get_bounding_box(void* global_data) override;
        void handle_boundary(void* global_data) override;
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;
        virtual Enemy_State_Machine_Component* make_state_machine_instance();
        virtual void update_state_machine_component(State_Machine_Component* state_machine);
        virtual void update_collision_component(Collision_Component* collision);
        virtual void clean_collision_data(Collision_Component* collision);
        virtual void init_skill_components();
        virtual void update_ui_attrubutes();
        virtual void remove_from_battle(void* global_data);
        virtual Custom::Anchor_Point get_origin_animation_anchor_point();

        const unsigned char ORIGIN_ATTACKED_IMAGE_OPACITY = 80;

        Character_Animation* enemy_animation = nullptr;
        Node* container = nullptr;
        Image_UI_Node* attacked_image = nullptr;
        Progression_Node* progression_health = nullptr;
        Node* progression_container = nullptr;
        glm::vec2 velosity{0.f, 0.f};

    private:
        const int PROGRESSION_HEALTH_TAG = 5;

        const int JUMP_ACTION_TAG = 0;
        const int HITTED_ACTION_TAG = 1;
        const int HIDE_PROGRESSION_HEALTH_TAG = 2;

        const Custom::Anchor_Point ORIGIN_ANIMATION_ANCHOR_POINT = {0.35, 0.5};
        const glm::vec2 ORIGIN_HEALTH_BAR_POSITION = {0, 80};
        const float DURATION_SHOW_PROGRESSION_HEALTH = 3.5f;
        const glm::vec2 ORIGIN_THUNDER_ANIMATION = {-5.f, 100.f};
        const glm::vec2 ORIGIN_THUNDER_SCALE = {1.5f, 0.85f};
        const glm::vec2 ORIGIN_FIRE_RUN_ANIMATION_POSITION = {-5.f, 180.f};
        const unsigned char ORIGIN_FIRE_RUN_ANIMATION_OPACITY = 220;
        const glm::vec2 END_FIRE_RUN_ANIMATION_POSITION = {-5.f, -40.f};

        void init_container();
        void init_enemy_animation();
        void init_components();
        void init_attacked_image();
        void init_progression_health(void* global_data);
        void setup_state_machine_component(State_Machine_Component* state_machine);

        void start_move(void* global_data);
        void start_attack(void* global_data);
        void start_hitted(void* global_data, int source_call_state);
        void start_dead(void* global_data);
        void start_stun(void* global_data, int source_call_state);
        void start_jump(void* global_data);

        void end_move(void* global_data);
        void end_attack(void* global_data);
        void end_hitted(void* global_data, int source_call_state);
        void end_dead(void* global_data);
        void end_stun(void* global_data, int source_call_state);
        void end_jump(void* global_data);

        void update_movement(float delta_time);
        void update_enemy_direction();
        void sync_attacked_image();
        void sync_progression_container();
        void update_colision_data();

        void action_enemy_jump(float delay, float duration, glm::vec2 character_position);
        void action_enemy_hitted(float delay, float duration, Const::DIRECTION bullet_direction, float percent_health);
        void action_enemy_dead(float delay, Layer_Node* label_exp_parent, float killed_exp);
        void action_enemy_hitted_by_thunder(float delay, float duration, Layer_Node* effect_layer);

        void hide_other_health_progression(Layer_Node* layer);

        int enemy_id = 0;
        float current_health = 0.f;

        /**
         * Debug
         */
        int call_clean_collision_data_count = 0;
    };
}  // namespace Meow_Meow