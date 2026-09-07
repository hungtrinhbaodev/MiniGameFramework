#include <meow_meow/animation/character_fire_run_animation.h>
#include <utils.h>

namespace Meow_Meow {
    Character_Fire_Run_Animation::Character_Fire_Run_Animation() {
        this->make_animation(
            "IDLE",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::IMMEDIATE
        );
        this->set_rotation(ORIGIN_ANIMATION_ROTATION);
        this->play_animation("IDLE");
        Utils::save_transform_origin(this);
    }

    Character_Fire_Run_Animation::~Character_Fire_Run_Animation() {}

}  // namespace Meow_Meow