#pragma once
#include <custom.h>
#include <image_node.h>
#include <label_node.h>
#include <scene_node.h>

namespace Exercises {
    class Movement_Scene : public Scene_Node {
    public:
        Movement_Scene();
        ~Movement_Scene();

        void on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) override;

    protected:
        void attach(void* global_data) override;

    private:
        const int ACTION_SHOW_LABEL_PRESSED_TAG = 0;
        const int ACTION_MOVE_IMAGE_TAG = 1;
        const int ACTION_SCALE_IMAGE_TAG = 2;
        const int ACTION_ROTATION_FOREVER_IMAGE_TAG = 3;

        const std::string IMAGE_PATH = "res/meow_meow/Characters/C13/Idle/01.png";
        const float DURATION_MOVE = 2.f;
        const float DURATION_SCALE = 1.f;
        const float DURATION_ROTATE_FOREVER = 1.f;
        const glm::vec2 START_POSITION = {128, 329};
        const glm::vec2 END_POSITION = {728, 145};
        const glm::vec2 START_SCALE = {1.f, 1.f};
        const glm::vec2 END_SCALE = {1.5f, 1.5f};
        const float ROTATION_FOREVER_PER_SECOND = 360;

        const Custom::Color LABEL_KEY_PRESSED_COLOR = {160, 20, 20};

        void init_image();
        void init_label_key_pressed();
        void action_show_key_pressed(std::string key_pressed);

        Image_Node* image = nullptr;
        Label_Node* label_key_pressed = nullptr;
    };
}  // namespace Exercises