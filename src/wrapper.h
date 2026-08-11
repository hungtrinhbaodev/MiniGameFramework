#pragma once
#include <custom.h>

#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <queue>
#include <string>

#include "raylib.h"

struct Image_Info {
    Custom::Size size;
};

struct Text_Info {
    Custom::Size size;
};

namespace Libs_Wrapper {
    void init_libs();

    bool is_debug_mode();

    float get_screen_width();

    float get_screen_height();

    void open_window(int width, int height, int FPS, std::string window_name, void* window = nullptr);

    bool window_should_close(void* window = nullptr);

    void close_window(void* window = nullptr);

    void draw_image(
        std::string image_path,
        Custom::Draw_Attributes attributes,
        bool enable_force_color = false,
        Custom::Color force_color = {}
    );

    void draw_text(std::string font_path, std::string text, int font_size, Custom::Draw_Attributes attributes);

    void draw_line(
        float start_x, float start_y, float end_x, float end_y, int draw_index, Custom::Color color, float thin = 1.0f
    );

    void start_draw_clipping(float width, float height, Custom::Draw_Attributes attributes);

    void end_draw_clipping(int draw_index);

    Image_Info image_info(std::string path);

    Text_Info text_info(std::string text, std::string font_path, int font_size);

    void draw_frame();

    void clear_libs();
}  // namespace Libs_Wrapper
