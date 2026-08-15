#pragma once
#include <image_node.h>

namespace Meow_Meow {
    void fix_background_to_scene(Image_Node* bg, Custom::Size screen_size);

    void run_action_idle_logo(Node* logo, glm::vec2 origin_scale);
};