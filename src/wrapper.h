#pragma once
#include <string>
#include <map>
#include <queue>

struct Image_Info
{
    int width;
    int height;
};

namespace Libs_Wrapper
{
    void init_libs();

    void open_window(int width, int height, int FPS, std::string window_name, void *window = nullptr);

    bool window_should_close(void *window = nullptr);

    void close_window(void *window = nullptr);

    void draw_image(std::string path, int x, int y, int width, int height);

    Image_Info image_info(std::string path);

    void draw_frame();

    void clear_libs();
}
