#pragma once
#include <custom.h>
#include <image_ui_node.h>
#include <layer_node.h>

class Progression_Node : public Image_UI_Node {
public:
    static Progression_Node* make(
        std::string image_bg,
        Custom::Rectangle_Area cap_insets,
        Custom::Size renderer_size,
        Custom::Color inner_progression_color,
        glm::vec2 inner_padding = {0.f, 0.f},
        glm::vec2 inner_delta_position = {0.f, 0.f}
    );

    Progression_Node();
    ~Progression_Node();

    float get_percent();
    Node_Type get_type() override;

    void set_inner_padding(glm::vec2 padding);
    void set_percent(float percent);
    void set_progression_color(Custom::Color color);
    void set_inner_delta_position(glm::vec2);
    void set_image(std::string image);
    void set_cap_insets(float x, float y, float width, float height);
    void set_renderer_size(Custom::Size renderer_size);

protected:
    void fix_update(float delta_time, void* global_data) override;

private:
    Layer_Node* clipping_node = nullptr;
    /**Use this color instead of inner node when don't have it*/
    Custom::Color progression_color{};
    Image_UI_Node* inner_progression = nullptr;
    /**
     * TODO: implement later when having resource
     * */
    Image_UI_Node* inner_custom_progression = nullptr;
    glm::vec2 inner_padding = {0.f, 0.f};
    glm::vec2 inner_delta_positon = {0.f, 0.f};
    float percent = 0.0f;

    void sync_inner_progression();
};