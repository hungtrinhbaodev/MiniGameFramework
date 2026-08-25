#pragma once
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/object/game_object.h>

namespace Meow_Meow {
    class Enemy_Node : public Game_Object {
    public:
        Enemy_Node();
        ~Enemy_Node();

    protected:
        Custom::Transformed_Rectangle get_bounding_box(void* global_data) override;
        void handle_boundary(void* global_data) override;
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;
        void on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) override;

    private:
        const int JUMP_ACTION_TAG = 0;
        const int HITTED_ACTION_TAG = 1;
        const Custom::Anchor_Point ORIGIN_ANIMATION_ANCHOR_POINT = {0.35, 0.5};
        const unsigned char ORIGIN_ATTACKED_IMAGE_OPACITY = 80;

        void init_container();
        void init_enemy_animation();
        void init_components();
        void init_attacked_image();

        void change_to_walk(void* global_data);
        void change_to_attack(void* global_data);
        void change_to_jump(void* global_data);
        void change_to_hitted(void* global_data, Const::DIRECTION bullet_direction);
        void handle_collision(float delta_time, void* global_data);
        void handle_state_machine(float delta_time, void* global_data);
        void update_movement(float delta_time);
        void update_enemy_direction();
        void sync_attacked_image();
        void update_colision_data();

        void action_enemy_jump(float delay, float duration, glm::vec2 character_position);
        void action_enemy_hitted(float delay, float duration, Const::DIRECTION bullet_direction);

        int enemy_animation_id = Const::BASE_ENEMY_ID + 1;
        Character_Animation* enemy_animation = nullptr;
        Image_UI_Node* attacked_image = nullptr;
        Node* container = nullptr;
        glm::vec2 velosity{0.f, 0.f};
    };
}  // namespace Meow_Meow