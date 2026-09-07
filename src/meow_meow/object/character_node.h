#pragma once
#include <image_ui_node.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/const.h>
#include <meow_meow/object/game_object.h>

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
        const int ACTION_FLIGHT_TAG = 3;
        const int ACTION_STUN_TAG = 4;

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

        void handle_level_up(void* global_data);
        void handle_collision(float delta_time, void* global_data);
        void handle_key_board(float delta_time, void* global_data);
        void handle_state_machine(float delta_time, void* global_data);
        void update_moverment(float delta_time);
        void update_character_direction();
        void sync_attacked_image();
        void sync_player_data(void* global_data);

        glm::vec2 get_direction();

        void change_to_move(
            Const::DIRECTION horizontal, Const::DIRECTION vertical, float duration_hold, void* global_data
        );
        void change_to_idle();
        void change_to_attack(void* global_data);
        void change_to_hitted(float damage, glm::vec2 enemy_direction, void* global_data);
        void change_to_invincible(void* global_data);
        void change_to_dead(void* global_data);
        void change_to_dash(void* global_data);
        void change_to_using_thunder_skill(void* global_data);
        void change_to_flight(void* global_data, float skill_damage);
        void change_to_stun(void* global_data);

        void action_character_hitted(float delay, float duration_hitted, glm::vec2 enemy_direction);
        void action_character_invincible(float delay, float duration);
        void action_character_dead(float delay, float dead_duration);
        void action_character_level_up(float delay);
        void action_character_dashing(float delay, float dash_duration, float dash_distance);
        void action_character_channelling_skill_thunder(float delay, float duration);
        void action_character_flight(float delay, float duration_fly);
        void action_character_stunned(float delay, float duration_stun);

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