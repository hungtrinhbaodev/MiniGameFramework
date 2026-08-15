#pragma once
#include <animation_node.h>
#include <button_node.h>
#include <image_node.h>
#include <meow_meow/ui/chosen_character_ui.h>
#include <scene_node.h>

namespace Meow_Meow {
    class Chosen_Character_Scene : public Scene_Node {
    public:
        Chosen_Character_Scene();
        ~Chosen_Character_Scene();
        void on_change_character(int next_character);

    protected:
        void attach(void* global_data) override;

    private:
        void init_bg();
        void init_logo();
        void init_lights();
        void init_btn_side(Animation_Node*& btn, bool is_left);
        void init_button_start();
        void init_characters(void* global_data);
        void update_btn_side_state(Animation_Node* btn, bool is_disable);
        void effect_show_scene();
        float effect_show_lights(float delay);
        float effect_show_logo(float delay);

        Custom::Color ORIGIN_BG_COLOR = {80, 80, 80};
        unsigned char ORIGIN_BG_OPACITY = 200;
        glm::vec2 ORIGIN_RATIO_POSITION_LOGO = {0.085, 0.9};
        glm::vec2 ORIGIN_SCALE_LOGO = {0.15, 0.15};
        float ORIGIN_POSITION_Y_BUTTON_START = 70;
        glm::vec2 ORIGIN_RATIO_POSITION_BUTTON_LEFT = {0.2, 0.45};
        glm::vec2 ORIGIN_RATIO_POSITION_BUTTON_RIGHT = {0.8, 0.45};
        glm::vec2 ORIGIN_POSITION_CHARACTER = {400, 200};
        Custom::Color DISABLE_COLOR_BUTTON_SIDES = {60, 60, 60};
        unsigned char DISABLE_OPACITY_BUTTON_SIDES = 220;
        char NUMBER_LIGHT = 2;
        glm::vec2 ORIGIN_LIGHT_LEFT_RATIO_POSITION = {0.1, 1.2};
        glm::vec2 ORIGIN_LIGHT_RIGHT_RATIO_POSITION = {0.9, 1.2};
        float ORIGIN_LIGHT_LEFT_ROTATION = -35;
        float ORIGIN_LIGHT_RIGHT_ROTATION = 35;
        unsigned char ORIGIN_LIGHT_OPACITY = 90;

        Image_Node* bg = nullptr;
        Image_Node* logo = nullptr;
        Button_Node* btn_start = nullptr;
        Animation_Node* btn_left = nullptr;
        Animation_Node* btn_right = nullptr;
        std::vector<Chosen_Character_UI*> characters;
        int current_character_index = 0;
        std::vector<Image_Node*> lights;
    };
}  // namespace Meow_Meow