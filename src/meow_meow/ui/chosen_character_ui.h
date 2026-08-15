#pragma once
#include <image_node.h>
#include <label_node.h>
#include <meow_meow/animation/character_animation.h>
#include <node.h>

namespace Meow_Meow {
    class Chosen_Character_UI : public Node {
    public:
        Chosen_Character_UI();
        Chosen_Character_UI(int character_id);
        ~Chosen_Character_UI();

    protected:
        void attach(void* global_data) override;

    private:
        void init_hall();
        void init_character();
        void init_character_name();

        glm::vec2 CHARACTER_SCALE = {1.f, 1.f};
        glm::vec2 ORIGIN_SCALE_HALL = {0.65, 0.65};
        glm::vec2 ORIGIN_CHACTER_POSITION = {0, 18 * CHARACTER_SCALE.y};
        Custom::Color HALL_COLOR = {100, 100, 100};
        glm::vec2 ORIGIN_CHARACTER_NAME_POSITION = {0, 120};
        Custom::Color CHARACTER_NAME_COLOR = {220, 220, 220};
        unsigned char CHARACTER_NAME_OPACITY = 200;

        Image_Node* hall;
        Character_Animation* character = nullptr;
        Label_Node* character_name = nullptr;
        int character_id = -1;
    };
}  // namespace Meow_Meow