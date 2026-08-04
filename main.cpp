#include "raylib.h"
#include <wrapper.h>
#include <utils.h>

const std::string PATH_RES = "C:/Work/MiniGameFramework/MiniGameFramework/";

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 480;
    void *window = nullptr;

    Libs_Wrapper::init_libs();
    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);

    while (!Libs_Wrapper::window_should_close(window))
    {
        Libs_Wrapper::draw_image(
            PATH_RES + "res/Png/CatBoxing/Idle/CatBoxing-Idle_00.png",
            0,
            0,
            1.f,
            1.f,
            0, 1);
        Libs_Wrapper::draw_frame();
    }

    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}