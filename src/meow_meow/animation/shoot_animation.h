#pragma once
#include <animation_node.h>
#include <meow_meow/const.h>

namespace Meow_Meow {
    class Shoot_Animation : public Animation_Node {
    public:
        Shoot_Animation();
        Shoot_Animation(glm::vec2 position, Const::DIRECTION horizontal_direction);
        ~Shoot_Animation();

    private:
        glm::vec2 ORIGIN_ANCHOR_POINT = {0.1, 0.5};
    };
}  // namespace Meow_Meow