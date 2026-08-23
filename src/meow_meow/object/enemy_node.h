#pragma once
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/object/game_object.h>

namespace Meow_Meow {
    class Enemy_Node : public Game_Object {
    public:
        Enemy_Node();
        ~Enemy_Node();

    protected:
        Custom::Transformed_Rectangle get_bounding_box() override;
        void handle_boundary(void* global_data) override;
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;

    private:
        const int JUMP_ACTION_TAG = 0;

        void init_enemy_animation();
        void init_components();

        void change_to_walk(void* global_data);
        void change_to_attack(void* global_data);
        void change_to_jump(void* global_data);
        void handle_state_machine(float delta_time, void* global_data);

        float effect_enemy_jump(float delay, float duration, glm::vec2 character_position);

        int enemy_animation_id = -1;
        Character_Animation* enemy_animation = nullptr;
        glm::vec2 direction{0.f, 0.f};
        glm::vec2 velosity{0.f, 0.f};
    };
}  // namespace Meow_Meow