#pragma once
#include <label_node.h>
#include <layer_node.h>

namespace Meow_Meow {
    class Start_Round_GUI : public Layer_Node {
    public:
        Start_Round_GUI();
        ~Start_Round_GUI();

        void show();

    protected:
        void fix_update(float delta_time, void* global_data);

    private:
        const int ACTION_SHOW_LABEL_TAG = 0;

        const Custom::Color BACKGROUND_COLOR = {0, 0, 0};
        const unsigned char ORIGIN_BACKGROUND_OPACITY = 180;
        const Custom::Color LABEL_COLOR = {245, 10, 230};
        const int LABEL_FONT_SIZE = 75;
        const glm::vec2 ORIGIN_LABEL_RATIO_POSITION = {0.5f, 0.5f};

        float effect_run_label_round(float delay);
        void init_label_round();

        Label_Node* label_start_round = nullptr;
    };
}  // namespace Meow_Meow