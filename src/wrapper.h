#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "raylib.h"

struct Image_Info {
    int width;
    int height;
};

struct Text_Info {
    float width;
    float height;
};

struct Draw_Attributes {
    float x;
    float y;
    float scale_x;
    float scale_y;
    float rotation;
    int draw_index;
    float anchor_x;
    float anchor_y;
    Color tint = WHITE;
    unsigned char opacity = 255;
    bool is_flipped_x = false;
    bool is_flipped_y = false;
    friend std::ostream& operator<<(std::ostream& os, const Draw_Attributes& attr) {
        os << (int)attr.tint.a << ", " << (int)attr.tint.b << ", " << (int)attr.tint.g << ", " << (int)attr.opacity;
        return os;
    }
};

namespace Libs_Wrapper {
    void init_libs();

    int get_screen_width();

    int get_screen_height();

    void open_window(int width, int height, int FPS, std::string window_name, void* window = nullptr);

    bool window_should_close(void* window = nullptr);

    void close_window(void* window = nullptr);

    void draw_image(std::string image_path, Draw_Attributes attributes);

    void draw_text(std::string font_path, std::string text, int font_size, Draw_Attributes attributes);

    void draw_line(
        float start_x, float start_y, float end_x, float end_y, int draw_index, glm::vec3 color, float thin = 1.0f
    );

    void start_draw_clipping(float width, float height, Draw_Attributes attributes);

    void end_draw_clipping(int draw_index);

    Image_Info image_info(std::string path);

    Text_Info text_info(std::string text, std::string font_path, int font_size);

    void draw_frame();

    void clear_libs();
}  // namespace Libs_Wrapper
