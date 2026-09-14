#pragma once
#include <image_ui_node.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/const.h>
#include <meow_meow/object/game_object.h>
#include <state_machine_component.h>

#include <map>

namespace Meow_Meow {
    class Character_Node : public Game_Object {
    public:
        Character_Node();
        ~Character_Node();

        bool is_moving_at_direction(Const::DIRECTION direction);

    protected:
        void fix_update(float delta_time, void* global_data) override;
        void attach(void* global_data) override;

        Custom::Transformed_Rectangle get_bounding_box(void* global_data) override;
        void handle_boundary(void* global_data) override;

    private:
        const int ACTION_HITTED_TAG = 0;
        const int ACTION_INVINCIBLE_TAG = 1;
        const int ACTION_DASHING_TAG = 2;
        const int ACTION_DEAD_TAG = 3;
        const int ACTION_FLIGHT_TAG = 4;
        const int ACTION_STUN_TAG = 5;
        const int ACTION_SHAKE_AND_STUN_TAG = 6;

        const unsigned char ORIGIN_ATTACKED_IMAGE_OPACITY = 100;
        const int NUMBER_FADE_IN_INVINCIBLE_STATE = 3;
        const unsigned char INVISIBLE_OPACITY = 120;
        std::map<int, glm::vec2> DELTA_POSITION_BULLET_BY_CHARACTER_ID = {
            {0, glm::vec2(18, -12)}, {1, glm::vec2(40, -5)}, {2, glm::vec2(30, -25)}
        };
        const glm::vec2 LEVEL_UP_POSITION = glm::vec2(0, 50);
        const glm::vec2 ORIGIN_SCALE_CHANNELLING_ANIMATION = glm::vec2(1.5f, 1.5f);
        const unsigned char ORIGIN_OPACITY_FIRE_BALL_ANIMATION = 210;

        void init_container();
        void init_components();
        void init_character_animation();
        void init_attacked_image();
        void set_up_state_machine_states(State_Machine_Component* state_machine);

        void handle_level_up(void* global_data);
        void update_moverment(float delta_time);
        void update_character_direction();
        void sync_attacked_image();
        void sync_player_data(void* global_data);
        glm::vec2 get_direction();

        void start_idle();
        void start_move(void* global_data);
        void start_attack(void* global_data);
        void start_hitted(void* global_data, int source_call_state);
        void start_invincible(void* global_data);
        void start_dead(void* global_data);
        void start_dash(void* global_data, int source_call_state);
        void start_channelling(void* global_data, int source_call_state);
        void start_flight(void* global_data, int source_call_state);
        void start_stun(void* global_data, int source_call_state);

        void end_move(void* global_data, std::string next_state);
        void end_attack(void* global_data);
        void end_hitted(void* global_data, int source_call_state);
        void end_invincible(void* global_data);
        void end_dead(void* global_data);
        void end_dash(void* global_data, int source_call_state);
        void end_channelling(void* global_data, int source_call_state);
        void end_flight(void* global_data, int source_call_state);
        void end_stun(void* global_data, int source_call_state);

        void action_character_hitted(float delay, float duration_hitted, glm::vec2 enemy_direction);
        void action_character_invincible(float delay, float duration);
        void action_character_dead(float delay, float dead_duration);
        void action_character_level_up(float delay);
        void action_character_dashing(float delay, float dash_duration, float dash_distance);
        void action_character_channelling_skill_thunder(float delay, float duration);
        void action_character_flight(float delay, float duration_fly);
        void action_character_stunned(float delay, float duration_stun);
        void action_character_shake_and_stun_when_enemy_thrown_hitted(
            float delay, float duration_shake_and_stun, float shake_distance
        );

        Character_Animation* character_animation = nullptr;
        Image_UI_Node* attacked_image = nullptr;
        /**
         * @Note: this not make a role as a container to do effect
         * hitted, invincible,... to avoid the change logic of position
         * in main node!
         */
        Node* container = nullptr;
        Const::DIRECTION horizontal_direction = Const::DIRECTION::RIGHT;
        Const::DIRECTION vertical_direction = Const::DIRECTION::UP;
        glm::vec2 velocity{0, 0};
        glm::vec2 accelarate{0, 0};
        int player_level = 0;
    };
}  // namespace Meow_Meow