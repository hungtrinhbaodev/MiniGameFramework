#pragma once
#include <button_node.h>
#include <image_node.h>
#include <layer_node.h>

namespace Meow_Meow {
    class Lose_Game_GUI : public Layer_Node {
    public:
        Lose_Game_GUI();
        ~Lose_Game_GUI();

    protected:
        void attach(void* global_data) override;

    private:
        const std::string PATH_IMAGE_LOSE_GAME = "res/meow_meow/LosePopUp.png";
        const glm::vec2 ORIGIN_IMAGE_LOSE_GAME_SCALE = {0.6f, 0.6f};
        const Custom::Color BACKGROUND_COLOR = {0, 0, 0};
        const unsigned char ORIGIN_BACKGROUND_OPACITY = 220;
        const glm::vec2 ORIGIN_RATIO_POSITION_BUTTON_BACK = {0.5f, 0.2f};
        const glm::vec2 ORIGIN_RATIO_POSITION_IMAGE_LOSE = {0.5f, 0.55f};

        void init_image_lose_game();
        void init_btn_back();

        float effect_show_image_lose(float delay);
        float effect_show_btn_back(float delay);

        Image_Node* image_lose = nullptr;
        Button_Node* btn_back = nullptr;
    };
}  // namespace Meow_Meow