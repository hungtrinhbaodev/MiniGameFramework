#pragma once
#include <meow_meow/animation/character_animation.h>
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

        bool finish_moving();
        void handle_key_board(float delta_time, void* global_data);
        void handle_state_machine(float delta_time, void* global_data);
        // void handle_collision(float delta_time, void* global_data);
        void update_moverment(float delta_time);

        int VELOCITY_MOVE_VALUE = 150;
        int ACCELARATE_MOVE_VALUE = 65;

        Character_Animation* character_animtion = nullptr;
        glm::vec2 velocity{0, 0};
        glm::vec2 accelarate{0, 0};
        glm::vec2 direction{1, 1};
    };
}  // namespace Meow_Meow