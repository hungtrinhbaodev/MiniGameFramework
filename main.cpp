#include <actions.h>
#include <animation_node.h>
#include <utils.h>
#include <wrapper.h>

#include "raylib.h"

const std::string PATH_RES = "C:/Work/MiniGameFramework/MiniGameFramework/";

int main(void) {
    const int screen_width = 800;
    const int screen_height = 480;
    void* window = nullptr;

    Libs_Wrapper::init_libs();
    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);

    Animation_Node* animation = new Animation_Node();
    animation->make_animation(
        "IDLE", PATH_RES + "res/Png/Characters/C3/Idle/Character3-Idle_",
        20, 0.06, ".png");
    animation->make_animation(
        "SHOOT", PATH_RES + "res/Png/Characters/C3/Shoot/Character3-Shoot_",
        9, 0.08, ".png");
    animation->play_animation("IDLE", 0.8f);
    animation->set_y(50.f);

    Image_Node* child = new Image_Node(PATH_RES + "res/Png/Characters/C3/Shoot/Character3-Shoot_00.png");
    child->set_position({30.f, 30.f});
    animation->add_child(child);

    long start = Utils::now();
    while (!Libs_Wrapper::window_should_close(window)) {
        long current = Utils::now();
        float delta_time = (float)(current - start) / 1000;
        start = current;
        if (IsKeyPressed(KEY_A)) {
            animation->stop_action(5);
            animation->do_action(Actions::spawn({Actions::move_by(0.2 /*delta_time=*/, {-50.f, 0}, Action_Ease::LINEAR),
                                                 Actions::sequence({Actions::scale_to(0.1 /*delta_time=*/, {1.2f, 1.2f}, Action_Ease::LINEAR),
                                                                    Actions::scale_to(0.1 /*delta_time=*/, {1.f, 1.f}, Action_Ease::LINEAR)})}),
                                 5);
        } else if (IsKeyPressed(KEY_D)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
        } else if (IsKeyPressed(KEY_S)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR), 5);
        } else if (IsKeyPressed(KEY_W)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR), 5);
        } else if (IsKeyDown(KEY_A)) {
        }
        animation->travel(delta_time);
        Libs_Wrapper::draw_frame();
    }

    delete (animation);
    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}