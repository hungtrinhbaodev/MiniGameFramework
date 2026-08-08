#include <layer_node.h>
#include <math_custom.h>
#include <wrapper.h>

std::array<glm::vec2, 4> Layer_Node::Rectangle::apply(Base_Node::Transform& transform, glm::vec2 anchor) {
    glm::vec2 size = {width, height};
    glm::vec2 scale = transform.scale;
    size = {size.x * scale.x, size.y * scale.y};
    glm::vec2 position = transform.position;

    glm::vec2 bottom_left = position + Math::rotate(transform.rotation, {-anchor.x * size.x, -anchor.y * size.y});
    glm::vec2 bottom_right = position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.x, -anchor.y * size.y});
    glm::vec2 top_right =
        position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.x, (1 - anchor.y) * size.y});
    glm::vec2 top_left = position + Math::rotate(transform.rotation, {-anchor.x * size.x, (1 - anchor.y) * size.y});

    return {{bottom_left, bottom_right, top_right, top_left}};
}

int Layer_Node::Rectangle::draw_rectangle(
    Base_Node::Transform& transform, glm::vec2 anchor, int base_draw_index, glm::vec3 color, float thin
) {
    std::array<glm::vec2, 4> points = apply(transform, anchor);
    for (int i = 0; i < points.size(); i++) {
        int current = i;
        int next = (current + 1) % points.size();
        Libs_Wrapper::draw_line(
            points[current].x, points[current].y, points[next].x, points[next].y, base_draw_index++, color, thin
        );
    }
    return points.size();
}

Layer_Node::Layer_Node() {}

Layer_Node::Layer_Node(glm::vec2 size) {
    this->size = size;
}

Layer_Node::Layer_Node(float size_x, float size_y) {
    set_content_size(size_x, size_y);
}

Layer_Node::~Layer_Node() {}

glm::vec2 Layer_Node::get_content_size() {
    return this->size;
}

void Layer_Node::set_content_size(glm::vec2 size) {
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

void Layer_Node::before_draw_children(Transform& world_transform, int& draw_index) {
    if (this->is_show_boundary) {
        glm::vec3 color = {255, 0, 0};
        Rectangle rec{this->size.x, this->size.y};
        draw_index += rec.draw_rectangle(world_transform, transform.anchor, draw_index);
    }

    if (this->is_clipping) {
        Libs_Wrapper::start_draw_clipping(
            this->size.x,
            this->size.y,
            {world_transform.position.x,
             world_transform.position.y,
             world_transform.scale.x,
             world_transform.scale.y,
             world_transform.rotation,
             draw_index,
             this->transform.anchor.x,
             this->transform.anchor.y,
             {255, 255, 255},
             world_transform.opacity}
        );
        draw_index++;
    }
}

void Layer_Node::after_draw_children(Transform& world_transform, int& draw_index) {
    if (this->is_clipping) {
        Libs_Wrapper::end_draw_clipping(draw_index);
        draw_index++;
    }
}
