#pragma once
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/const.h>
#include <node.h>

namespace Meow_Meow {
    class Character_Node : public Node {
    public:
        Character_Node();
        ~Character_Node();

    protected:
        void fix_update(float delta_time, void* global_data) override;
        void attach(void* global_data) override;

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
        void change_to_move(Const::DIRECTION horizontal, Const::DIRECTION vertical);
        void change_to_idle();
        void change_to_attack(void* global_data);

        int VELOCITY_MOVE_VALUE = 150;
        int ACCELARATE_MOVE_VALUE = 65;

        Character_Animation* character_animtion = nullptr;
        Const::DIRECTION horizontal_direction = Const::DIRECTION::RIGHT;
        Const::DIRECTION vertical_direction = Const::DIRECTION::UP;
        glm::vec2 velocity{0, 0};
        glm::vec2 accelarate{0, 0};
    };
}  // namespace Meow_Meow