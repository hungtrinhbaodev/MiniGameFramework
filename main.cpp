#include "raylib.h"
#include <wrapper.h>
#include <utils.h>
#include <image_node.h>

const std::string PATH_RES = "D:/MiniGameFramework/MiniGameFramework/";

int main(void)
{
    const int screen_width = 800;
    const int screen_height = 480;
    void *window = nullptr;

    Libs_Wrapper::init_libs();
    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);

    Base_Node* image = new Image_Node(PATH_RES + "res/Png/Characters/C3/Idle/Character3-Idle_00.png");
    image->set_position({100.f, 100.f});
    image->set_scale({0.8f, 0.8f});

    long start = Utils::now();
    while (!Libs_Wrapper::window_should_close(window))
    {
        long current = Utils::now();
        float delta_time = (float)(current - start) / 1000;
        start = current;
        image->travel(delta_time);
        Libs_Wrapper::draw_frame();
    }

    delete(image);
    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}