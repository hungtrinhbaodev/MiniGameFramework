#include <meow_meow/animation/level_up_flame_animation.h>

namespace Meow_Meow {
    Level_Up_Flame_Animation::Level_Up_Flame_Animation() {
        this->make_animation(
            "LEVEL_UP",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::IMMEDIATE,
            ".png"
        );
        this->play_animation("LEVEL_UP");
        this->set_scale(ORIGIN_FLAME_SCALE);
        this->on_finish_animation_callback("LEVEL_UP", [](Base_Node* target, void*) { target->remove_from_parent(); });
    }

    Level_Up_Flame_Animation::~Level_Up_Flame_Animation() {}
}  // namespace Meow_Meow