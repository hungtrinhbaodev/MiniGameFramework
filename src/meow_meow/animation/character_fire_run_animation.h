#pragma once
#include <animation_node.h>

#include <glm/glm.hpp>

namespace Meow_Meow {
    class Character_Fire_Run_Animation : public Animation_Node {
    public:
        Character_Fire_Run_Animation();
        ~Character_Fire_Run_Animation();

    private:
        float ORIGIN_ANIMATION_ROTATION = -90.f;
        float DURATION_ANIMATION_LOOP = 0.06;
        int NUMBER_ANIMATION_FRAME = 8;
        std::string ANIMATION_IMAGE_PATH = "res/meow_meow/fire_run/Fire Spell_Frame_";
    };
}  // namespace Meow_Meow