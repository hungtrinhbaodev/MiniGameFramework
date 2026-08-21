#pragma once
#include <image_node.h>
#include <layer_node.h>
#include <meow_meow/object/character_node.h>

namespace Meow_Meow {
    class Battle_Layer : public Layer_Node {
    public:
        Battle_Layer();
        ~Battle_Layer();

        Image_Node* get_bg();

    private:
        void init_bg();
        void init_character();

        Character_Node* character = nullptr;
        Image_Node* bg = nullptr;
    };
}  // namespace Meow_Meow