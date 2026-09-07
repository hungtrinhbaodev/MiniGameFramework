#pragma once
#include <animation_node.h>

#include <glm/glm.hpp>

namespace Meow_Meow {
    class Character_Fire_Ball_Animation : public Animation_Node {
    public:
        Character_Fire_Ball_Animation();
        ~Character_Fire_Ball_Animation();

    private:
        float DURATION_ANIMATION_LOOP = 0.06;
        int NUMBER_ANIMATION_FRAME = 8;
        std::string ANIMATION_IMAGE_PATH = "res/meow_meow/fire_ball/Fire Ball_Frame_";
    };
}  // namespace Meow_Meow