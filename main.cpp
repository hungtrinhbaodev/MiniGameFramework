#include <actions.h>
#include <animation_node.h>
#include <utils.h>
#include <wrapper.h>

#include "raylib.h"

int main(void) {
    const int screen_width = 800;
    const int screen_height = 480;
    void* window = nullptr;

    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);
    Libs_Wrapper::init_libs();

    Node* scene = new Node();
    scene->set_position({0.f, 0.f});

    Animation_Node* animation = new Animation_Node();
    animation->make_animation(
        "IDLE", "res/Png/Characters/C3/Idle/Character3-Idle_", 20, 0.06, ".png"
    );
    animation->make_animation(
        "SHOOT", "res/Png/Characters/C3/Shoot/Character3-Shoot_", 9, 0.08, ".png"
    );
    animation->play_animation("IDLE", 0.8f);
    animation->set_y(50.f);
    animation->set_cascade_opacity(false);
    scene->add_child(animation);

    std::string path = "res/Png/Characters/C3/Shoot/Character3-Shoot_00.png";

    Image_Node* child = new Image_Node(path);
    child->set_position({30.f, 30.f});
    animation->add_child(child);

    Image_Node* child_run_1 = new Image_Node(path);
    child_run_1->set_visible(false);
    scene->add_child(child_run_1);

    Image_Node* child_run_2 = new Image_Node(path);
    child_run_2->set_visible(false);
    scene->add_child(child_run_2);

    Image_Node* child_run_3 = new Image_Node(path);
    child_run_3->set_visible(false);
    child_run_3->set_name("debug");
    scene->add_child(child_run_3);

    long start = Utils::now();
    while (!Libs_Wrapper::window_should_close(window)) {
        long current = Utils::now();
        float delta_time = (float)(current - start) / 1000;
        start = current;
        if (IsKeyPressed(KEY_A)) {
            animation->stop_action(5);
            animation->do_action(
                Actions::spawn(
                    Actions::move_by(0.2 /*delta_time=*/, {-50.f, 0}, Action_Ease::LINEAR),
                    Actions::sequence(
                        Actions::scale_to(0.1 /*delta_time=*/, {1.2f, 1.2f}, Action_Ease::LINEAR),
                        Actions::scale_to(0.1 /*delta_time=*/, {1.f, 1.f}, Action_Ease::LINEAR)
                    ),
                    Actions::sequence(
                        Actions::fade_to(
                            0.1 /*delta_time=*/, Utils::random_int(125, 220), Action_Ease::LINEAR
                        ),
                        Actions::fade_in(0.1 /*delta_time=*/, Action_Ease::LINEAR)
                    )
                ),
                5
            );
        } else if (IsKeyPressed(KEY_D)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
        } else if (IsKeyPressed(KEY_S)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR), 5);
        } else if (IsKeyPressed(KEY_W)) {
            animation->stop_action(5);
            animation->do_action(Actions::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR), 5);
        } else if (IsKeyDown(KEY_C)) {
            Base_Action* action = Actions::spawn(
                Actions::rotate_by(1.0f, 360),
                Actions::sequence(Actions::fade_to(0.5, 120), Actions::fade_in(0.5))
            );
            child->stop_action(5);
            child->do_action(Actions::sequence(Actions::show(), action->repeat_forever()), 5);
        } else if (IsKeyDown(KEY_X)) {
            child->do_action(Actions::hide());
        } else if (IsKeyDown(KEY_Z)) {
            child_run_1->stop_action(5);
            child_run_2->stop_action(5);
            child_run_3->stop_action(5);
            child_run_1->set_position({0.f, 50.f});
            child_run_2->set_position({0.f, 50.f});
            child_run_3->set_position({0.f, 50.f});
            child_run_1->do_action(
                Actions::sequence(
                    Actions::show(), Actions::move_to(1, {400.f, 240.f}, SINE_IN), Actions::hide()
                ),
                5
            );
            child_run_2->do_action(
                Actions::sequence(
                    Actions::show(), Actions::move_to(1, {400.f, 240.f}, SINE_OUT), Actions::hide()
                ),
                5
            );
            child_run_3->do_action(
                Actions::sequence(Actions::show(), Actions::move_to(1, {400.f, 240.f}, LINEAR)), 5
            );
        }
        scene->travel(delta_time);
        Libs_Wrapper::draw_frame();
    }

    delete (scene);
    Libs_Wrapper::close_window(window);
    Libs_Wrapper::clear_libs();

    return 0;
}