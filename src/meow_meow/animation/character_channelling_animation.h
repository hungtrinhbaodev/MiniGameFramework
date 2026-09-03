#pragma once
#include <animation_node.h>

namespace Meow_Meow {
    class Character_Channelling_Animation : public Animation_Node {
    public:
        Character_Channelling_Animation();
        Character_Channelling_Animation(float duration);
        ~Character_Channelling_Animation();

    private:
        float DURATION_ANIMATION_LOOP = 0.06;
        int NUMBER_ANIMATION_FRAME = 10;
        std::string ANIMATION_IMAGE_PATH = "res/meow_meow/Channelling/";
    };
}  // namespace Meow_Meow