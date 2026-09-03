#include <meow_meow/animation/skill_thunder_animation.h>

namespace Meow_Meow {

    Skill_Thunder_Animation::Skill_Thunder_Animation() {
        this->make_animation(
            "EXPLOSE",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::IMMEDIATE
        );
        this->set_anchor(ORIGIN_ANCHOR_POINT);
        this->on_finish_animation_callback("EXPLOSE", [](Animation_Node* target, void* global_data) {
            target->remove_from_parent();
        });
    }

    Skill_Thunder_Animation::Skill_Thunder_Animation(float duration) : Skill_Thunder_Animation() {
        float animation_duration = this->get_amimation_duration("EXPLOSE");
        this->play_animation("EXPLOSE", duration / animation_duration);
    }

    Skill_Thunder_Animation::~Skill_Thunder_Animation() {}

}  // namespace Meow_Meow