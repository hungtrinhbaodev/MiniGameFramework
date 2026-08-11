#include <actions.h>
#include <animation_node.h>
#include <image_ui_node.h>
#include <label_node.h>
#include <layer_node.h>
#include <math_custom.h>
#include <scene_node.h>
#include <utils.h>
#include <wrapper.h>

#include "raylib.h"

Scene_Node* scene = nullptr;
Animation_Node* animation = nullptr;
Layer_Node* layer = nullptr;
Animation_Node* animation_2 = nullptr;

Layer_Node* layer_1 = nullptr;
Layer_Node* layer_2 = nullptr;
Layer_Node* layer_3 = nullptr;
Label_Node* label = nullptr;
Image_UI_Node* ui = nullptr;
Image_Node* image = nullptr;

void start_test_node() {
    scene = new Scene_Node();
    scene->set_position({0.0, 0.0});

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
    // animation_2->set_scale_x(-1.f);

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

    layer = new Layer_Node{{800.f, 480.f}};
    layer->set_rotation(10.f);
    layer->set_show_boundary(true);
    layer->set_clipping(true);

    label = new Label_Node("Trinh Bao Hung", "res/fonts/KnightWarrior-w16n8.otf", 28);
    label->set_anchor({0.5, 0.5});
    label->set_position({0.f, 200.f});
    label->set_color({220, 50, 100});
    label->set_scale({1.f, 1.f});
    label->do_action(Actions::sequence(Actions::rotate_by(1, 360, Action_Ease::SINE_OUT))->repeat_forever());

    ui = new Image_UI_Node();
    ui->set_image("res/Png/Ui/AddonBoxNumber.png");
    ui->set_enable_nine_scale(true);
    ui->set_cap_insets(30, 20, 74, 11);
    ui->set_position({400.f, 240.f});
    ui->set_renderer_size({200.f, 51.f});

    image = new Image_Node();
    image->set_image("res/Png/Ui/AddonBtnyellow.png");
    image->set_position({100.f, 200.f});

    layer->add_child(sub_layer);
    scene->add_child(layer);
    // sub_layer->add_child(animation_2);
    layer->add_child(label);
    scene->add_child(ui);
    // scene->add_child(image);
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
        animation_2->set_flipped_x(true);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
        animation_2->set_flipped_x(false);
    } else if (IsKeyPressed(KEY_UP)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR), 5);
    } else if (IsKeyPressed(KEY_DOWN)) {
        animation_2->stop_action(5);
        animation_2->do_action(Actions::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR), 5);
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