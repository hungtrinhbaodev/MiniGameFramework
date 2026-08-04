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
        for (int i = 0; i < 1000; i++)
        {
            Libs_Wrapper::draw_image(PATH_RES + "res/Png/CatBoxing/Idle/CatBoxing-Idle_00.png", Utils::random_int(0, screen_width), Utils::random_int(0, screen_height), 0, 0);
        }
        Libs_Wrapper::draw_frame();
    }

    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}