#pragma once
#include <image_node.h>
#include <node.h>
#include <progression_node.h>

namespace Meow_Meow {
    class Battle_Wave_Progression_UI : public Node {
    public:
        Battle_Wave_Progression_UI();
        ~Battle_Wave_Progression_UI();

        float get_ui_width();

    protected:
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;

    private:
        const int ACTION_IDLE_ARROW_TAG = 0;

        const std::string PATH_IMAGE_ENEMY = "res/meow_meow/enemy_head.png";
        const glm::vec2 ORIGIN_IMAGE_ENEMY_SCALE = {0.65f, 0.65f};
        const std::string PATH_IMAGE_ARROW = "res/meow_meow/Uplogo10.png";
        const glm::vec2 ORIGIN_IMAGE_ARROW_SCALE = {0.65f, 0.65f};
        const std::string PATH_IMAGE_PROGRESSION_WAVE = "res/meow_meow/CoinBar.png";
        const Custom::Color PROGRESSION_WAVE_COLOR = {240, 240, 5};
        const glm::vec2 PROGRESSION_INNER_DELTA_POSITION = {0.f, 1.f};
        const glm::vec2 PROGRESSION_INNER_PADDING = {12.f, 13.f};
        const Custom::Rectangle_Area PROGRESSION_CAP_INSETS = {20.f, 10.f, 129.f, 36.f};
        const Custom::Size PROGRESSION_RENDERER_SIZE = {200.f, 45.f};
        const float GAP_WIDTH_UI = -5.f;
        const float GAP_HEIGHT_ARROW_WITH_PROGRESSION = -15.f;
        const float DELTA_POSITION_ARROW_X = 20.f;
        const unsigned char OPACITY_DISABLE_ARROW = 220;
        const Custom::Color COLOR_DISABLE_ARROW = {120, 120, 120};

        void init_image_enemy();
        void init_wave_progression();
        void init_image_wave_arrows(void* global_data);

        void on_idle_arrow_at_current_wave();

        Progression_Node* wave_progression = nullptr;
        Image_Node* image_enemy = nullptr;
        std::vector<Image_Node*> image_wave_arrows;
        int current_wave = -1;
    };
}  // namespace Meow_Meow