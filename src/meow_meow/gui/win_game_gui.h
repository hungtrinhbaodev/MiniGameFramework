#pragma once
#include <button_node.h>
#include <image_node.h>
#include <layer_node.h>
#include <meow_meow/ui/chosen_character_ui.h>

namespace Meow_Meow {
    class Win_Game_GUI : public Layer_Node {
    public:
        Win_Game_GUI();
        ~Win_Game_GUI();

    protected:
        void attach(void* global_data) override;

    private:
        const std::string PATH_IMAGE_WIN_GAME = "res/meow_meow/WinPopUp.png";
        const glm::vec2 ORIGIN_IMAGE_WIN_GAME_SCALE = {0.6f, 0.6f};
        const Custom::Color BACKGROUND_COLOR = {0, 0, 0};
        const unsigned char ORIGIN_BACKGROUND_OPACITY = 220;
        const glm::vec2 ORIGIN_RATIO_POSITION_IMAGE_WIN = {0.5f, 0.55f};
        const glm::vec2 ORIGIN_RATIO_POSITION_BUTTON_BACK = {0.5f, 0.2f};
        const glm::vec2 ORIGIN_RATIO_POSITION_CHARACTER = {0.5f, 0.48f};

        void init_image_win_game();
        void init_character_ui();
        void init_btn_back();

        float effect_show_image_win(float delay);
        float effect_show_btn_back(float delay);
        float effect_show_character(float delay);

        Image_Node* image_win = nullptr;
        Chosen_Character_UI* character = nullptr;
        Button_Node* btn_back = nullptr;
    };
}  // namespace Meow_Meow