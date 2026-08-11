#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace Custom {

    struct Size {
        float width = 0.f;
        float height = 0.f;
        friend std::ostream& operator<<(std::ostream& os, const Size& size) {
            os << "Size: " << "width: " << size.width << ", height: " << size.height << std::endl;
            return os;
        }
    };

    struct Anchor_Point {
        float x = 0.f;
        float y = 0.f;
        friend std::ostream& operator<<(std::ostream& os, const Anchor_Point& anchor) {
            os << "Anchor: " << "width: " << anchor.x << ", height: " << anchor.y << std::endl;
            return os;
        }
    };

    struct Color {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
        friend std::ostream& operator<<(std::ostream& os, const Color& color) {
            os << "Color: " << "red: " << (int)color.r << ", green: " << (int)color.g << ", blue: " << (int)color.b
               << std::endl;
            return os;
        }
    };

    struct Flipped {
        bool x = false;
        bool y = false;
        friend std::ostream& operator<<(std::ostream& os, const Flipped& flipped) {
            os << "Flipped: " << "x: " << flipped.x << ", y: " << flipped.y << std::endl;
            return os;
        }
    };

    struct Transform {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation = 0.0f;
        unsigned char opacity = 255;
        Color color;  // TODO: use later.
        Flipped flipped;

        void forward(const Transform& other, bool is_cascade_opacity, Flipped forward_flipped);
        void inverse(const Transform& other, unsigned char inverse_opacity, Flipped inverse_flipped);

        // Builder
        Transform& set_position(glm::vec2 position);
        Transform& set_scale(glm::vec2 scale);
        Transform& set_rotation(float rotation);
        Transform& set_opacity(unsigned char opacity);
        Transform& set_color(Color color);
        Transform& set_flipped(Flipped flipped);
        Transform& set_flipped_x(bool flipped_x);
        Transform& set_flipped_y(bool flipped_x);

        friend std::ostream& operator<<(std::ostream& os, const Transform& transform) {
            os << "Transform: " << "x: " << transform.position.x << ", y: " << transform.position.y
               << ", scale_x: " << transform.scale.x << ", scale_y: " << transform.scale.y
               << ", rotation: " << transform.rotation << std::endl;
            return os;
        }
    };

    struct Rectangle_Area {
        float x = 0.f;
        float y = 0.f;
        float width = 0.f;
        float height = 0.f;

        friend std::ostream& operator<<(std::ostream& os, const Rectangle_Area& area) {
            os << "Rectangle_Area: " << "x: " << area.x << ", y: " << area.y << ", width: " << area.width
               << ", height: " << area.height << std::endl;
            return os;
        }
    };

    struct Rectangle {
        Size size;
        Rectangle(float size_x, float size_y);
        std::array<glm::vec2, 4> apply(const Transform& transform, const Anchor_Point& anchor);
        int draw_rectangle(
            const Transform& transform,
            const Anchor_Point& anchor,
            int base_draw_index = 0,
            Color color = {255, 0, 0},
            float thin = 3.f
        );
    };

    struct Draw_Attributes {
        Transform transform;
        Anchor_Point anchor{0.f, 0.f};
        int draw_index = 0;
        Color tint{255, 255, 255};
        bool is_use_rect_texture = false;
        Rectangle_Area rect_texture = {0.f, 0.f, 0.f, 0.f};
        friend std::ostream& operator<<(std::ostream& os, const Draw_Attributes& attr) {
            os << "draw_index: " << attr.draw_index << std::endl
               << attr.transform << attr.anchor << attr.tint << attr.rect_texture;
            return os;
        }
    };

    struct Distance_1D {
        float x;
        float length;
    };

}  // namespace Custom