#include "raylib.h"
#include <wrapper.h>
#include <utils.h>

const std::string PATH_RES = "D:/MiniGameFramework/MiniGameFramework/";

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 480;
    void *window = nullptr;

    Libs_Wrapper::init_libs();
    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);

    Draw_Attributes attributes = {
        0.0f,
        50.0f,
        1.0f,
        1.0f,
        5.0f,
        10,
        0.5f,
        0.5f
    };

    Draw_Attributes text_attributes = attributes;
    text_attributes.tint = BLACK;
    text_attributes.z_order = 9;

    while (!Libs_Wrapper::window_should_close(window))
    {
        Libs_Wrapper::draw_image(
            PATH_RES + "res/Png/Characters/C3/Idle/Character3-Idle_00.png",
            attributes
        );
        Libs_Wrapper::draw_text(
            "",
            "Hi toi la hung",
            18,
            text_attributes
        );
        Libs_Wrapper::draw_frame();
    }

    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}