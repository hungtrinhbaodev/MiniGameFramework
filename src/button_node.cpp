#include <actions.h>
#include <button_node.h>

int Button_Node::ACTION_PRESS_TAG = 5;

Button_Node* Button_Node::make(
    std::string image_path,
    std::string text,
    std::function<void(Button_Node*)> pressed_caller,
    Custom::Size renderer_size,
    Custom::Rectangle_Area cap_insets,
    Custom::Color color,
    std::string font_path,
    int font_size
) {
    Button_Node* button = new Button_Node();
    button->set_image(image_path);
    if (renderer_size.width > -1 && renderer_size.height > -1) {
        button->set_enable_nine_scale(true);
        button->set_renderer_size(renderer_size);
        button->set_cap_insets(cap_insets.x, cap_insets.y, cap_insets.width, cap_insets.height);
    }
    button->update_label(font_path, font_size, color);
    button->set_text(text);
    button->on_pressed(pressed_caller);
    return button;
}

Button_Node::Button_Node() {
    this->inner_label = new Label_Node();
    this->inner_label->set_anchor({0.5f, 0.5f});
    this->add_child(this->inner_label);

    this->set_touch_enabled(true);
    this->set_swallow_touches(true);
    this->set_touched_caller([this](glm::vec2 position, Base_Node* target) {
        Button_Node* button = reinterpret_cast<Button_Node*>(target);
        button->run_pressed_action();
        if (button->pressed_caller) {
            button->pressed_caller(this);
        }
    });
}

Button_Node::~Button_Node() {}

void Button_Node::update_label(std::string font, int font_size, Custom::Color color) {
    std::cout << "Button_Node::update_label " << font_size << std::endl;
    this->inner_label->set_font(font);
    this->inner_label->set_font_size(font_size);
    this->inner_label->set_color(color);
}

void Button_Node::set_text(std::string text) {
    this->inner_label->set_text(text);
}

void Button_Node::on_pressed(std::function<void(Button_Node*)> caller) {
    this->pressed_caller = caller;
}

void Button_Node::set_image(std::string image) {
    Image_Node::set_image(image);
    this->sync_inner_label();
}

void Button_Node::set_renderer_size(Custom::Size size) {
    Image_UI_Node::set_renderer_size(size);
    this->sync_inner_label();
}

void Button_Node::run_pressed_action() {
    this->stop_action(Button_Node::ACTION_PRESS_TAG);
    this->set_scale({1, 1});
    this->do_action(
        Action::sequence(
            Action::scale_to(0.06, {1.1, 1.1}, Action_Ease::SINE_OUT),
            Action::scale_to(0.06, {1.0, 1.0}, Action_Ease::SINE_IN)
        ),
        Button_Node::ACTION_PRESS_TAG
    );
}

void Button_Node::sync_inner_label() {
    Custom::Size size = get_renderer_size();
}