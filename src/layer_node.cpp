#include <layer_node.h>

Layer_Node::Layer_Node() {}

Layer_Node::Layer_Node(glm::vec2 size) {
    this->size = size;
}

Layer_Node::Layer_Node(float size_x, float size_y) {
    set_content_size(size_x, size_y);
}

Layer_Node::~Layer_Node() {}

void Layer_Node::set_content_size(glm::vec2 size) {
    this->size = size;
}

void Layer_Node::set_content_size(float size_x, float size_y) {
    set_content_size({size_x, size_y});
}

void Layer_Node::set_clipping(bool is_clipping) {
    this->is_clipping = is_clipping;
}

void Layer_Node::before_draw_children(Transform& world_transform, int& draw_index) {
    if (is_clipping) {
        draw_index++;
    }
}

void Layer_Node::after_draw_children(Transform& world_transform, int& draw_index) {
    if (is_clipping) {
        draw_index++;
    }
}
