#include <meow_meow/animation/shoot_animation.h>
#include <meow_meow/const.h>

namespace Meow_Meow {

    Shoot_Animation::Shoot_Animation() {
        this->make_animation("FIRE", "res/meow_meow/ShootFx/Fx2-animation_", 15, 0.04);
        this->on_finish_animation_callback("FIRE", [](Animation_Node* target, void*) { target->remove_from_parent(); });
    }

    Shoot_Animation::Shoot_Animation(glm::vec2 position, Const::DIRECTION horizontal_direction) {
        this->make_animation(
            "FIRE",
            ANIMATION_IMAGE_PATH,
            NUMBER_ANIMATION_FRAME,
            DURATION_ANIMATION_LOOP,
            Animation_Node::ANIMATION_LOAD_MODE::ASYNC
        );
        glm::vec2 anchor = ORIGIN_ANCHOR_POINT;
        anchor.x = horizontal_direction == Const::DIRECTION::LEFT ? (1 - anchor.x) : anchor.x;
        this->set_anchor(anchor);
        this->on_finish_animation_callback("FIRE", [](Animation_Node* target, void*) { target->remove_from_parent(); });
        this->set_position(position);
        this->play_animation("FIRE");
        this->set_flipped_x(horizontal_direction == Const::DIRECTION::LEFT);
        this->set_tag(Const::SHOOT_ANIMATION_NODE_TAG);
    }

    Shoot_Animation::~Shoot_Animation() {}

}  // namespace Meow_Meow