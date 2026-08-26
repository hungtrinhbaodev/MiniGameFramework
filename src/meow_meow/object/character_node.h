#pragma once
#include <image_ui_node.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/const.h>
#include <meow_meow/object/game_object.h>

namespace Meow_Meow {
    class Character_Node : public Game_Object {
    public:
        Character_Node();
        ~Character_Node();

        void set_character_animation_id(int character_animation_id);

    protected:
        void fix_update(float delta_time, void* global_data) override;
        void attach(void* global_data) override;

        Custom::Transformed_Rectangle get_bounding_box(void* global_data) override;
        void handle_boundary(void* global_data) override;

    private:
        const int ACTION_HITTED_TAG = 0;
        const int ACTION_INVINCIBLE_TAG = 1;

        const unsigned char ORIGIN_ATTACKED_IMAGE_OPACITY = 100;
        const int NUMBER_FADE_IN_INVINCIBLE_STATE = 3;
        const unsigned char INVISIBLE_OPACITY = 120;
        glm::vec2 DELTA_POSITION_BULLET = glm::vec2(18, -12);

        void init_container();
        void init_components();
        void init_character_animation();
        void init_attacked_image();

        void handle_collision(float delta_time, void* global_data);
        void handle_key_board(float delta_time, void* global_data);
        void handle_state_machine(float delta_time, void* global_data);
        void update_moverment(float delta_time);
        void update_character_direction();
        void sync_attacked_image();

        bool is_moving_at_direction(Const::DIRECTION direction);
        glm::vec2 get_direction();

        void change_to_move(
            Const::DIRECTION horizontal, Const::DIRECTION vertical, float duration_hold, void* global_data
        );
        void change_to_idle();
        void change_to_attack(void* global_data);
        void change_to_hitted(float damage, glm::vec2 enemy_direction, void* global_data);
        void change_to_invincible(void* global_data);
        void change_to_dead(void* global_data);

        void action_character_hitted(float delay, float duration_hitted, glm::vec2 enemy_direction);
        void action_character_invincible(float delay, float duration);
        void action_character_dead(float delay, float dead_duration);

        int character_animation_id = 0;
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
    };
}  // namespace Meow_Meow