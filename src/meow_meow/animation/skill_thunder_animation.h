#pragma once
#include <animation_node.h>

namespace Meow_Meow {
    class Skill_Thunder_Animation : public Animation_Node {
    public:
        Skill_Thunder_Animation();
        Skill_Thunder_Animation(float duration);
        ~Skill_Thunder_Animation();

    private:
        const glm::vec2 ORIGIN_ANCHOR_POINT = {0.5f, 1.f};
        float DURATION_ANIMATION_LOOP = 0.05;
        int NUMBER_ANIMATION_FRAME = 17;
        std::string ANIMATION_IMAGE_PATH = "res/meow_meow/Thunder/";
    };
}  // namespace Meow_Meow