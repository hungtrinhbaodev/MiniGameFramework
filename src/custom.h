#pragma once
#include <glm/glm.hpp>
#include <iostream>

namespace Custom {

    struct Size {
        float width = 0.f;
        float height = 0.f;
    };

    struct Anchor_Point {
        float x;
        float y;
    };

    struct Color {
        unsigned char r = 255;
        unsigned char g = 255;
        unsigned char b = 255;
    };

    struct Flipped {
        bool x = false;
        bool y = false;
    };

    struct Transform {
        glm::vec2 position{0.0f, 0.0f};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation = 0.0f;
        unsigned char opacity = 255;
        glm::u8vec3 color{255, 255, 255};

        void forward(const Transform& other, bool is_cascade_opacity);
        void inverse(const Transform& other, unsigned char inverse_opacity);
    };

    struct Rectangle {
        Size size;
        Rectangle(float size_x, float size_y);
        std::array<glm::vec2, 4> apply(const Transform& transform, Anchor_Point anchor);
        int draw_rectangle(
            const Transform& transform,
            Anchor_Point anchor,
            int base_draw_index = 0,
            glm::vec3 color = {255.f, 0.f, 0.f},
            float thin = 3.f
        );
    };

    struct Draw_Attributes {
        Transform transform;
        Anchor_Point anchor;
        int draw_index;
        Color tint;
        Flipped flipped;
        friend std::ostream& operator<<(std::ostream& os, const Draw_Attributes& attr) {
            return os;
        }
    };

}  // namespace Custom