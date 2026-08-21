#include <custom.h>
#include <math_custom.h>
#include <utils.h>
#include <wrapper.h>

namespace Custom {

    std::string Color::to_string() {
        std::string color;
        color += std::string("red: ") + std::to_string(this->r);
        color += std::string("green: ") + std::to_string(this->g);
        color += std::string("blue: ") + std::to_string(this->b);
        return color;
    }

    void Transform::forward(const Transform& other, bool is_cascade_opacity, Flipped forward_flipped) {
        this->flipped = forward_flipped;
        position += Math::rotate(rotation, other.position * scale);
        rotation += other.rotation;
        scale.x *= other.scale.x;
        scale.y *= other.scale.y;
        this->opacity = is_cascade_opacity ? std::min(this->opacity, other.opacity) : other.opacity;
    }

    void Transform::inverse(const Transform& other, unsigned char inverse_opacity, Flipped inverse_flipped) {
        this->flipped = inverse_flipped;
        rotation -= other.rotation;
        scale.x /= other.scale.x;
        scale.y /= other.scale.y;
        position -= Math::rotate(rotation, other.position * scale);
        opacity = inverse_opacity;
    }

    Transform& Transform::set_position(glm::vec2 position) {
        this->position = position;
        return *this;
    }

    Transform& Transform::set_scale(glm::vec2 scale) {
        this->scale = scale;
        return *this;
    }

    Transform& Transform::set_rotation(float rotation) {
        this->rotation = rotation;
        return *this;
    }

    Transform& Transform::set_opacity(unsigned char opacity) {
        this->opacity = opacity;
        return *this;
    }

    Transform& Transform::set_color(Color color) {
        this->color = color;
        return *this;
    }

    Transform& Transform::set_flipped(Flipped flipped) {
        this->flipped = flipped;
        return *this;
    }

    Transform& Transform::set_flipped_x(bool flipped_x) {
        this->flipped.x = flipped_x;
        return *this;
    }

    Transform& Transform::set_flipped_y(bool flipped_y) {
        this->flipped.y = flipped_y;
        return *this;
    }

    Rectangle::Rectangle(float size_x, float size_y) {
        this->size = {size_x, size_y};
    }

    std::array<glm::vec2, 4> Rectangle::apply(const Transform& transform, const Anchor_Point& anchor) {
        glm::vec2 scale = transform.scale;
        size = {size.width * scale.x, size.height * scale.y};
        glm::vec2 position = transform.position;

        glm::vec2 bottom_left =
            position + Math::rotate(transform.rotation, {-anchor.x * size.width, -anchor.y * size.height});
        glm::vec2 bottom_right =
            position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.width, -anchor.y * size.height});
        glm::vec2 top_right =
            position + Math::rotate(transform.rotation, {(1 - anchor.x) * size.width, (1 - anchor.y) * size.height});
        glm::vec2 top_left =
            position + Math::rotate(transform.rotation, {-anchor.x * size.width, (1 - anchor.y) * size.height});

        return {{bottom_left, bottom_right, top_right, top_left}};
    }

    int Rectangle::draw_border_rectangle(
        const Transform& transform,
        const Anchor_Point& anchor,
        int base_draw_index,
        Color color,
        float thin,
        bool is_dashed
    ) {
        std::array<glm::vec2, 4> points = apply(transform, anchor);
        for (int i = 0; i < points.size(); i++) {
            int current = i;
            int next = (current + 1) % points.size();
            Libs_Wrapper::draw_line(
                points[current].x,
                points[current].y,
                points[next].x,
                points[next].y,
                base_draw_index++,
                color,
                thin,
                is_dashed
            );
        }
        return points.size();
    }

    int Rectangle::draw_rectangle(
        const Transform& transform, const Anchor_Point& anchor, int base_draw_index, Color color, unsigned char opacity
    ) {
        Custom::Transform copied_transform = transform;
        copied_transform.opacity = opacity;
        Libs_Wrapper::draw_rectangle(size.width, size.height, {copied_transform, anchor, base_draw_index, color});
        return 1;
    }

    void Rectangle_Area::fix_with(const Rectangle_Area& other) {
        this->x = std::max(other.x, this->x);
        this->y = std::max(other.y, this->y);
        if (this->width <= other.width && this->height <= other.height) {
            if (this->x + this->width > other.width) {
                this->x = other.width - this->width;
            }
            if (this->y + this->height > other.height) {
                this->y = other.height - this->height;
            }
        } else if (this->width > other.width && this->height > other.height) {
            this->x = 0;
            this->y = 0;
            this->width = other.width;
            this->height = other.height;
        } else {
            if (this->height > other.height) {
                this->y = 0;
                this->height = other.height;
                this->x = other.width - this->width;
            } else {
                this->x = 0;
                this->width = other.width;
                this->y = other.height - this->height;
            }
        }
    }

    std::string Transformed_Rectangle::to_string() {
        std::string str;
        for (int i = 0; i < points.size(); i++) {
            str += "point " + std::to_string(i) + ": " + Utils::vec2_to_string(points[i]) + " ";
        }
        return str;
    }

    bool Transformed_Rectangle::is_in_area(glm::vec2 point) const {
        std::vector<glm::vec2> convex(std::begin(points), std::end(points));
        return Math::is_in_convex(convex, point);
    }

    bool Transformed_Rectangle::is_collision_with(const Transformed_Rectangle& other) const {
        for (int i = 0; i < other.points.size(); i++) {
            if (is_in_area(other.points[i])) {
                return true;
            }
        }
        for (int i = 0; i < points.size(); i++) {
            if (other.is_in_area(points[i])) {
                return true;
            }
        }
        return false;
    }

    float Transformed_Rectangle::distance_from(const Transformed_Rectangle& other) const {
        std::vector<glm::vec2> my_convex(std::begin(points), std::end(points));
        std::vector<glm::vec2> other_convex(std::begin(other.points), std::end(other.points));
        glm::vec2 my_center = Math::get_convex_center(my_convex);
        glm::vec2 other_center = Math::get_convex_center(other_convex);
        return glm::distance(my_center, other_center);
    }

}  // namespace Custom