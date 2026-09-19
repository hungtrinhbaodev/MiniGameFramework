#include <director.h>
#include <exercises/movement_scene.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/scene/battle_scene.h>
#include <meow_meow/scene/chosen_character_scene.h>
#include <meow_meow/scene/loading_scene.h>
#include <test/image_test.h>
#include <test/test_scene.h>
#include <utils.h>
#include <wrapper.h>

#include "raylib.h"

int main(void) {
    const int screen_width = 800;
    const int screen_height = 480;
    void* window = nullptr;

    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);
    Libs_Wrapper::init_libs();

    /**
     * @TODO: don't know how to make an good entry so just manual do it
     * first by add your scene here, after that find the wait to have
     * a smooth flow to entry better!
     * */
    // Director::get()->start(new Image_Test_Scene(), nullptr);
    Director::get()->start(new Meow_Meow::Loading_Scene(), Meow_Meow::Global_Data::get());
    // Director::get()->start(new Meow_Meow::Battle_Scene(), Meow_Meow::Global_Data::get());
    // Director::get()->start(new Test_Scene(), Meow_Meow::Global_Data::get());
    // Director::get()->start(new Exercises::Movement_Scene(), nullptr);

    long start = Utils::now();
    while (!Libs_Wrapper::window_should_close(window)) {
        long current = Utils::now();
        float delta_time = (float)(current - start) / 1000;
        start = current;
        Libs_Wrapper::start_frame();
        Director::get()->start_frame();
        Libs_Wrapper::handle_frame(delta_time);
        Director::get()->process_frame(delta_time);
        Libs_Wrapper::draw_frame();
        Director::get()->end_frame();
    }

    /**
     * @TODO: in future add dispatcher to clean this flow clear singleton data
     * from now just clear manualy!
     */
    Meow_Meow::Global_Data::clear();
    Director::get()->end();
    Libs_Wrapper::clear_libs();
    Libs_Wrapper::close_window(window);
    Director::clear();

    return 0;
}