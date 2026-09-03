#pragma once
#include <animation_node.h>

#include <glm/glm.hpp>

namespace Meow_Meow {
    class Level_Up_Flame_Animation : public Animation_Node {
    public:
        Level_Up_Flame_Animation();
        ~Level_Up_Flame_Animation();

    private:
        glm::vec2 ORIGIN_FLAME_SCALE = {0.75f, 0.75f};
        float DURATION_ANIMATION_LOOP = 0.035;
        int NUMBER_ANIMATION_FRAME = 27;
        std::string ANIMATION_IMAGE_PATH = "res/meow_meow/LevelUpFlame/";
    };
}  // namespace Meow_Meow