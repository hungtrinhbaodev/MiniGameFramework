#include <custom.h>
#include <math_custom.h>
#include <wrapper.h>

namespace Custom {
    void Transform::forward(const Transform& other, bool is_cascade_opacity) {
        position += Math::rotate(rotation, other.position * scale);
        rotation += other.rotation;
        scale.x *= other.scale.x;
        scale.y *= other.scale.y;
        this->opacity = is_cascade_opacity ? std::min(this->opacity, other.opacity) : other.opacity;
    }

    void Transform::inverse(const Transform& other, unsigned char inverse_opacity) {
        rotation -= other.rotation;
        scale.x /= other.scale.x;
        scale.y /= other.scale.y;
        position -= Math::rotate(rotation, other.position * scale);
        opacity = inverse_opacity;
    }

    Rectangle::Rectangle(float size_x, float size_y) {
        this->size = {size_x, size_y};
    }

    std::array<glm::vec2, 4> Rectangle::apply(const Transform& transform, Anchor_Point anchor) {
        glm::vec2 size = {this->size.width, this->size.height};
        glm::vec2 scale = transform.scale;
        size = {size.x * scale.x, size.y * scale.y};
        glm::vec2 position = transform.position;

        glm::vec2 bottom_left = position + Math::rotate(transform.rotation, {-anchor.x * size.x, -anchor.y * size.y});
        glm::vec2 bottom_right =
            position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.x, -anchor.y * size.y});
        glm::vec2 top_right =
            position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.x, (1 - anchor.y) * size.y});
        glm::vec2 top_left = position + Math::rotate(transform.rotation, {-anchor.x * size.x, (1 - anchor.y) * size.y});

        return {{bottom_left, bottom_right, top_right, top_left}};
    }

    int Rectangle::draw_rectangle(
        const Transform& transform, Anchor_Point anchor, int base_draw_index, glm::vec3 color, float thin
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

}  // namespace Custom