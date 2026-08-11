#include <layer_node.h>
#include <math_custom.h>
#include <wrapper.h>

Layer_Node::Layer_Node() {}

Layer_Node::Layer_Node(Custom::Size size) {
    this->size = size;
}

Layer_Node::Layer_Node(float size_x, float size_y) {
    set_content_size(size_x, size_y);
}

Layer_Node::~Layer_Node() {}

Custom::Size Layer_Node::get_content_size() {
    return this->size;
}

void Layer_Node::set_content_size(Custom::Size size) {
    this->size = size;
}

void Layer_Node::set_content_size(float size_x, float size_y) {
    set_content_size({size_x, size_y});
}

void Layer_Node::set_show_boundary(bool is_show_boundary) {
    this->is_show_boundary = is_show_boundary;
}

void Layer_Node::set_clipping(bool is_clipping) {
    this->is_clipping = is_clipping;
}

void Layer_Node::before_draw_children(Custom::Transform& world_transform, int& draw_index) {
    if (this->is_show_boundary || Libs_Wrapper::is_debug_mode()) {
        glm::vec3 color = {255, 0, 0};
        Custom::Rectangle rec{this->size.width, this->size.height};
        draw_index += rec.draw_rectangle(world_transform, anchor, draw_index);
    }

    if (this->is_clipping) {
        Libs_Wrapper::start_draw_clipping(this->size.width, this->size.height, {world_transform, anchor, draw_index});
        draw_index++;
    }
}

void Layer_Node::after_draw_children(Custom::Transform& world_transform, int& draw_index) {
    if (this->is_clipping) {
        Libs_Wrapper::end_draw_clipping(draw_index);
        draw_index++;
    }
}
