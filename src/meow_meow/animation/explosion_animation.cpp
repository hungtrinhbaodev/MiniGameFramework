#include <defined.h>
#include <meow_meow/animation/explosion_animation.h>

namespace Meow_Meow {
    Explosion_Animation::Explosion_Animation() {}

    Explosion_Animation::Explosion_Animation(glm::vec2 explosion_position) {
        this->make_animation(
            "EXPLOSION",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::ASYNC
        );
        this->on_finish_animation_callback("EXPLOSION", [](Base_Node* target, void* global_data) {
            target->remove_from_parent();
        });
        this->set_position(explosion_position);
        this->play_animation("EXPLOSION");
        this->set_opacity(ORIGIN_OPACITY);
        this->set_scale(ORIGIN_EXPLOSION_SCALE);
    }

    Explosion_Animation::~Explosion_Animation() {}
}  // namespace Meow_Meow