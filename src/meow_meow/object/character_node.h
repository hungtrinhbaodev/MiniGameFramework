#pragma once
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

        Custom::Transformed_Rectangle get_bounding_box() override;
        void handle_boundary(void* global_data) override;

    private:
        void init_components();
        void init_character_animation();

        void handle_key_board(float delta_time, void* global_data);
        void handle_state_machine(float delta_time, void* global_data);
        // void handle_collision(float delta_time, void* global_data);

        void update_moverment(float delta_time);
        void update_character_direction();

        bool is_moving_at_direction(Const::DIRECTION direction);
        glm::vec2 get_direction();

        void change_to_move(Const::DIRECTION horizontal, Const::DIRECTION vertical, float duration_hold = 0);
        void change_to_idle();
        void change_to_attack(void* global_data);

        float VELOCITY_MOVE_VALUE = 150;
        float ACCELARATE_MOVE_VALUE = 65;
        float DURATION_ATTACK = 0.5;

        float BONUS_VELOSITY_RATE = 0.25;
        float BONUS_VELOSITY = 10;
        float MAX_BONUS_VELOSITY = 60;

        glm::vec2 DELTA_POSITION_BULLET = glm::vec2(18, -12);

        int character_animation_id = 0;
        Character_Animation* character_animtion = nullptr;
        Const::DIRECTION horizontal_direction = Const::DIRECTION::RIGHT;
        Const::DIRECTION vertical_direction = Const::DIRECTION::UP;
        glm::vec2 velocity{0, 0};
        glm::vec2 accelarate{0, 0};
    };
}  // namespace Meow_Meow