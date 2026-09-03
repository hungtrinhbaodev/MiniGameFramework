#include <meow_meow/animation/character_channelling_animation.h>

namespace Meow_Meow {

    Character_Channelling_Animation::Character_Channelling_Animation() {
        this->make_animation(
            "EXPLOSE",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::IMMEDIATE
        );
        this->on_finish_animation_callback("EXPLOSE", [](Animation_Node* target, void* global_data) {
            target->remove_from_parent();
        });
    }

    Character_Channelling_Animation::Character_Channelling_Animation(float duration)
        : Character_Channelling_Animation() {
        float animation_duration = this->get_amimation_duration("EXPLOSE");
        this->play_animation("EXPLOSE", duration / animation_duration);
    }

    Character_Channelling_Animation::~Character_Channelling_Animation() {}

}  // namespace Meow_Meow