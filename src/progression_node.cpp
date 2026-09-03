#include <progression_node.h>
#include <utils.h>

Progression_Node* Progression_Node::make(
    std::string image_bg,
    Custom::Rectangle_Area cap_insets,
    Custom::Size renderer_size,
    Custom::Color inner_progression_color,
    glm::vec2 inner_padding,
    glm::vec2 inner_delta_position
) {
    Progression_Node* progression = new Progression_Node();
    progression->set_image(image_bg);
    progression->set_cap_insets(cap_insets.x, cap_insets.y, cap_insets.width, cap_insets.height);
    progression->set_progression_color(inner_progression_color);
    progression->set_inner_padding(inner_padding);
    progression->set_inner_delta_position(inner_delta_position);
    progression->set_renderer_size(renderer_size);
    return progression;
}

Progression_Node::Progression_Node() {
    this->set_enable_nine_scale(true);
    this->set_cascade_opacity(true);
    this->clipping_node = new Layer_Node();
    this->inner_progression = new Image_UI_Node();
    this->inner_progression->set_enable_force_renderer_color(true);
    this->inner_progression->set_enable_nine_scale(true);
    this->clipping_node->set_clipping(true);
    this->clipping_node->add_child(this->inner_progression);
    this->clipping_node->set_cascade_opacity(true);
    this->add_child(this->clipping_node);
}

Progression_Node::~Progression_Node() {}

float Progression_Node::get_percent() {
    return this->percent;
}

Node_Type Progression_Node::get_type() {
    return Node_Type::PROGRESSION;
}

void Progression_Node::set_inner_padding(glm::vec2 padding) {
    this->inner_padding = padding;
}

void Progression_Node::set_percent(float percent) {
    this->percent = std::max(std::min(percent, 100.f), 0.f);
    Custom::Size renderer_size = this->get_renderer_size();
    this->clipping_node->set_content_size({renderer_size.width * percent / 100, renderer_size.height});
}

void Progression_Node::set_progression_color(Custom::Color color) {
    this->inner_progression->set_force_renderer_color(color);
}

void Progression_Node::set_inner_delta_position(glm::vec2 delta_position) {
    this->inner_delta_positon = delta_position;
}

void Progression_Node::set_image(std::string image) {
    Image_Node::set_image(image);
    this->inner_progression->set_image(image);
}

void Progression_Node::set_cap_insets(float x, float y, float width, float height) {
    Image_UI_Node::set_cap_insets(x, y, width, height);
    this->inner_progression->set_cap_insets(x, y, width, height);
}

void Progression_Node::set_renderer_size(Custom::Size renderer_size) {
    Image_UI_Node::set_renderer_size(renderer_size);
}

void Progression_Node::fix_update(float delta_time, void* global_data) {
    this->sync_inner_progression();
    Image_UI_Node::fix_update(delta_time, global_data);
}

void Progression_Node::sync_inner_progression() {
    Custom::Size size = this->get_renderer_size();
    this->clipping_node->set_content_size({size.width, size.height});
    this->clipping_node->set_position({-size.width * anchor.x, -size.height * anchor.y});
    glm::vec2 inner_position = {size.width / 2, size.height / 2};
    this->inner_progression->set_cap_insets(this->get_cap_insets());
    this->inner_progression->set_renderer_size({size.width - inner_padding.x, size.height - inner_padding.y});
    inner_position += this->inner_delta_positon;
    this->inner_progression->set_position(inner_position);
    this->set_percent(this->get_percent());
}