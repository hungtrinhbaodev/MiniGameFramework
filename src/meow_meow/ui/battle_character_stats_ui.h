#pragma once
#include <custom.h>
#include <label_node.h>
#include <layer_node.h>
#include <meow_meow/animation/character_animation.h>
#include <progression_node.h>

namespace Meow_Meow {
    class Battle_Character_Stats_UI : public Layer_Node {
    public:
        Battle_Character_Stats_UI();
        ~Battle_Character_Stats_UI();

    protected:
        void attach(void* global_data) override;
        void fix_update(float delta_time, void* global_data) override;
        void update_stats(void* global_data);

    private:
        const int ACTION_PROGRESS_HEALTH_TAG = 0;
        const float PROGRESSION_DURATION = 0.5f;

        const Custom::Size LAYER_SIZE = {320, 100};
        const Custom::Size LAYER_PADDING = {5, 8};
        const Custom::Size LAYER_GAP = {0, 3};

        const std::string PATH_BACKGROUND_BOX = "res/meow_meow/GemsBarBg.png";
        const Custom::Rectangle_Area CAP_INSETS_BACKGROUND_BOX = {30, 15, 129, 32};
        const Custom::Size SIZE_BACKGROUND_BOX = {100, 100};
        const glm::vec2 ANIMATION_CHARACTER_POSITION = {0, -15};

        const std::string PATH_PROGRESSION = "res/meow_meow/AddonBoxNumber.png";
        const Custom::Rectangle_Area CAP_INSETS_PROGRESSION = {30, 10, 74, 31};
        const Custom::Size SIZE_PROGRESSION = {200, 42};
        const Custom::Color HEALTH_COLOR_PROGRESSION = {20, 160, 20};
        const glm::vec2 PADDING_PROGRESSION = {6, 8};

        const std::string LABEL_PATH = "res/fonts/default.otf";
        const int DAMAGE_LABEL_FONT_SIZE = 22;
        const Custom::Color DAMAGE_LABEL_COLOR = {240, 20, 20};
        const int LEVEL_LABEL_FONT_SIZE = 20;
        const Custom::Color LEVEL_LABEL_COLOR = {20, 200, 20};

        void init_box_character();
        void init_character_stats();

        Character_Animation* character_animation = nullptr;
        Layer_Node* clipping_box = nullptr;
        Image_UI_Node* background_character = nullptr;

        Progression_Node* health_progression = nullptr;
        Label_Node* label_damage = nullptr;
        Label_Node* label_level = nullptr;

        int current_level = 1;
        float current_health = 0.f;
    };
}  // namespace Meow_Meow