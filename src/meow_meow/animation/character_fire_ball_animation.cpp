#include <meow_meow/animation/character_fire_ball_anmimation.h>

namespace Meow_Meow {
    Character_Fire_Ball_Animation::Character_Fire_Ball_Animation() {
        this->make_animation(
            "IDLE",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::IMMEDIATE
        );
        this->play_animation("IDLE");
    }

    Character_Fire_Ball_Animation::~Character_Fire_Ball_Animation() {}

}  // namespace Meow_Meow