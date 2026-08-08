#include <actions.h>
#include <animation_node.h>
#include <layer_node.h>
#include <math_custom.h>
#include <utils.h>
#include <wrapper.h>

#include "raylib.h"

Node* scene = nullptr;
Animation_Node* animation = nullptr;
Layer_Node* layer = nullptr;
Animation_Node* animation_2 = nullptr;

Layer_Node* layer_1 = nullptr;
Layer_Node* layer_2 = nullptr;
Layer_Node* layer_3 = nullptr;

void start_test_node() {
    scene = new Node();
    scene->set_position({0.f, 0.f});

    animation = new Animation_Node();
    animation->make_animation("IDLE", "res/Png/Characters/C3/Idle/Character3-Idle_", 20, 0.06, ".png");
    animation->make_animation("SHOOT", "res/Png/Characters/C3/Shoot/Character3-Shoot_", 9, 0.08, ".png");
    animation->play_animation("IDLE", 0.8f);
    animation->set_y(50.f);
    animation->set_cascade_opacity(false);
    animation->set_position({0.f, 0.f});

    animation_2 = new Animation_Node();
    animation_2->make_animation("IDLE", "res/Png/Characters/C5/Idle/Character5-Idle_", 20, 0.06, ".png");
    animation_2->make_animation("SHOOT", "res/Png/Characters/C5/Shoot/Character5-Shoot_", 10, 0.08, ".png");
    animation_2->play_animation("IDLE", 0.8f);
    animation_2->set_y(50.f);
    animation_2->set_cascade_opacity(false);
    animation_2->set_position({0.f, 0.f});

    layer = new Layer_Node{{800.f, 480.f}};
    layer->set_rotation(10.f);
    layer->set_show_boundary(true);
    layer->set_clipping(true);

    Layer_Node* sub_layer = new Layer_Node{{600.f, 320.f}};
    sub_layer->set_anchor({0.5f, 0.5f});
    sub_layer->set_position({200.f, 120.f});
    sub_layer->set_rotation(60.f);
    sub_layer->set_clipping(true);
    sub_layer->set_show_boundary(true);

    // layer_1 = new Layer_Node{{600.f, 320.f}};
    // layer_1->set_rotation(30.f);
    // layer_1->set_anchor({0.5f, 0.5f});
    // layer_1->set_position({50.5f, 60.5f});
    // layer_1->set_show_boundary(true);
    // layer_1->set_clipping(true);

    // layer_2 = new Layer_Node{{400.f, 620.f}};
    // layer_2->set_rotation(-20.f);
    // layer_2->set_anchor({0.5f, 0.5f});
    // layer_2->set_position({30.5f, 15.5f});
    // layer_2->set_show_boundary(true);
    // layer_2->set_clipping(true);

    // layer_3 = new Layer_Node{{200.f, 120.f}};
    // layer_3->set_rotation(-60.f);
    // layer_3->set_anchor({0.5f, 0.5f});
    // layer_2->set_position({240.5f, 30.5f});
    // layer_3->set_show_boundary(true);
    // layer_3->set_clipping(true);
    // layer_3->set_show_boundary(true);

    layer = new Layer_Node{{800.f, 480.f}};
    layer->set_rotation(10.f);
    layer->set_show_boundary(true);
    layer->set_clipping(true);

    layer->add_child(sub_layer);
    scene->add_child(layer);
    sub_layer->add_child(animation_2);
    // layer_1->add_child(layer_2);
    // layer_2->add_child(animation_2);
    // layer->add_child(animation_2);
    // layer_1->add_child(layer_2);
    // layer_2->add_child(layer_3);
    // layer_3->add_child(animation);
}

void loop_test_node(float delta_time) {
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
                    Actions::fade_to(0.1 /*delta_time=*/, Math::random_int(125, 220), Action_Ease::LINEAR),
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
    }

    if (IsKeyPressed(KEY_LEFT)) {
        animation_2->stop_action(5);
        animation_2->do_action(
            Actions::spawn(
                Actions::move_by(0.2 /*delta_time=*/, {-50.f, 0}, Action_Ease::LINEAR),
                Actions::sequence(
                    Actions::scale_to(0.1 /*delta_time=*/, {1.2f, 1.2f}, Action_Ease::LINEAR),
                    Actions::scale_to(0.1 /*delta_time=*/, {1.f, 1.f}, Action_Ease::LINEAR)
                ),
                Actions::sequence(
                    Actions::fade_to(0.1 /*delta_time=*/, Math::random_int(125, 220), Action_Ease::LINEAR),
                    Actions::fade_in(0.1 /*delta_time=*/, Action_Ease::LINEAR)
                )
            ),
            5
        );
    } else if (IsKeyPressed(KEY_RIGHT)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
    } else if (IsKeyPressed(KEY_UP)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR), 5);
    } else if (IsKeyPressed(KEY_DOWN)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR), 5);
    }
    scene->travel(delta_time);
}

void end_test_node() {
    delete (scene);
}

int main(void) {
    const int screen_width = 800;
    const int screen_height = 480;
    void* window = nullptr;

    Libs_Wrapper::open_window(screen_width, screen_height, 60, "Mini Game Framework", window);
    Libs_Wrapper::init_libs();

    start_test_node();

    long start = Utils::now();
    while (!Libs_Wrapper::window_should_close(window)) {
        long current = Utils::now();
        float delta_time = (float)(current - start) / 1000;
        start = current;
        loop_test_node(delta_time);
        Libs_Wrapper::draw_frame();
    }

    end_test_node();
    Libs_Wrapper::clear_libs();
    Libs_Wrapper::close_window(window);

    return 0;
}