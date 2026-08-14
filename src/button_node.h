#pragma once
#include <image_ui_node.h>
#include <label_node.h>

class Button_Node : public Image_UI_Node {
public:
    static Button_Node* make(
        std::string image_path,
        std::string text,
        std::function<void(Button_Node*, void*)> pressed_caller,
        Custom::Size renderer_size = {-1, -1},
        Custom::Rectangle_Area cap_insets = {0, 0, 0, 0},
        Custom::Color color = {165, 165, 165},
        std::string font_path = "",
        int font_size = 18
    );
    Button_Node();
    ~Button_Node();

    Node_Type get_type() override;

    void update_label(std::string font, int font_size, Custom::Color color);
    void set_text(std::string text);
    void on_pressed(std::function<void(Button_Node*, void*)> caller);

    void set_image(std::string image);
    void set_renderer_size(Custom::Size size);

private:
    static int ACTION_PRESS_TAG;
    Label_Node* inner_label = nullptr;
    std::function<void(Button_Node*, void*)> pressed_caller;

    void run_pressed_action();
    void sync_inner_label();
};