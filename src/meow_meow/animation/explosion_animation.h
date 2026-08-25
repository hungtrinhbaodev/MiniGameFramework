#pragma once
#include <animation_node.h>

namespace Meow_Meow {
    class Explosion_Animation : public Animation_Node {
    public:
        Explosion_Animation();
        Explosion_Animation(glm::vec2 explosion_position);
        ~Explosion_Animation();

    private:
        const glm::vec2 ORIGIN_EXPLOSION_SCALE = {0.2, 0.2};
        const unsigned char ORIGIN_OPACITY = 180;
    };
}  // namespace Meow_Meow