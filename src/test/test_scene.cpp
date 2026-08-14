#include <math_custom.h>
#include <test/test_scene.h>

#include "raylib.h"

Test_Scene::Test_Scene() {
    animation = new Animation_Node();
    animation->make_animation("IDLE", "res/meow_meow/Characters/C3/Idle/Character3-Idle_", 20, 0.06, ".png");
    animation->make_animation("SHOOT", "res/meow_meow/Characters/C3/Shoot/Character3-Shoot_", 9, 0.08, ".png");
    animation->play_animation("IDLE", 0.8f);
    animation->set_y(50.f);
    animation->set_cascade_opacity(false);
    animation->set_position({120.f, 200.f});
    Collision_Component* collison_1 = Collision_Component::make(
        COLISION_ANIMATION_TAG,
        {100, 135},
        0,
        {0, 0},
        nullptr,
        [](Base_Node* animation_1, void* global_data, std::vector<Collision_Information> collisioned) {
            std::cout << "Some one hit me animation_1: " << collisioned.size() << std::endl;
        }
    );
    animation->add_component(collison_1);

    animation_2 = new Animation_Node();
    animation_2->make_animation("IDLE", "res/meow_meow/Characters/C5/Idle/Character5-Idle_", 20, 0.06, ".png");
    animation_2->make_animation("SHOOT", "res/meow_meow/Characters/C5/Shoot/Character5-Shoot_", 10, 0.08, ".png");
    animation_2->play_animation("IDLE", 0.8f);
    animation_2->set_y(50.f);
    animation_2->set_cascade_opacity(false);
    animation_2->set_position({0.f, 0.f});
    // animation_2->set_scale_x(-1.f);
    Collision_Component* collison_2 = Collision_Component::make(
        COLISION_ANIMATION_TAG,
        {100, 135},
        0,
        {0, 0},
        nullptr,
        [](Base_Node* animation_2, void* global_data, std::vector<Collision_Information> collisioned) {
            std::cout << "Some one hit me animation_2: " << collisioned.size() << std::endl;
            /** test flow remove */
            animation_2->remove_from_parent(false);
        }
    );
    animation_2->add_component(collison_2);

    Layer_Node* sub_layer = new Layer_Node{{600.f, 320.f}};
    // sub_layer->set_anchor({0.5f, 0.5f});
    sub_layer->set_position({200.f, 120.f});
    sub_layer->set_rotation(0.f);
    sub_layer->set_clipping(true);
    sub_layer->set_show_boundary(true);

    layer = new Layer_Node{{800.f, 480.f}};
    // layer->set_rotation(10.f);
    layer->set_show_boundary(true);
    layer->set_clipping(false);

    label = new Label_Node("START!", "res/fonts/default.otf", 28);
    label->set_anchor({0.5, 0.5});
    label->set_position({0.f, 200.f});
    label->set_color({220, 50, 100});
    label->set_scale({1.f, 1.f});
    label->do_action(Action::sequence(Action::rotate_by(1, 360, Action_Ease::SINE_OUT))->repeat_forever());

    ui = new Image_UI_Node();
    ui->set_image("res/meow_meow/AddonBoxNumber.png");
    ui->set_enable_nine_scale(true);
    ui->set_cap_insets(30, 10, 74, 31);
    ui->set_position({400.f, 400.f});
    ui->set_renderer_size({160.f, 40.f});
    ui->set_scale({1.5f, 1.2f});
    ui->set_rotation(30);
    ui->do_action(Action::sequence(Action::rotate_by(3, 360, Action_Ease::SINE_OUT))->repeat_forever());

    ui_child = new Image_UI_Node();
    ui_child->set_image("res/Png/Ui/AddonBoxNumber.png");
    ui_child->set_enable_nine_scale(true);
    ui_child->set_cap_insets(30, 10, 74, 31);
    ui_child->set_renderer_size({160.f - 6.f, 40.f - 6.f});
    ui_child->set_enable_force_renderer_color(true);
    ui_child->set_force_renderer_color({20, 180, 20});
    ui->add_child(ui_child);

    image = new Image_Node();
    image->set_image("res/meow_meow/AddonBtnyellow.png");
    image->set_position({100.f, 200.f});
    image->set_anchor({0., 0.});

    image->set_touch_enabled(true);
    image->set_swallow_touches(true);
    image->set_touched_caller([](glm::vec2 touch_position, Base_Node* image, void* global_data) {
        std::cout << "Hi there, pressed me 1: " << touch_position.x << ", " << touch_position.y << std::endl;
    });

    image2 = new Image_Node();
    image2->set_image("res/meow_meow/AddOnSlotBtn.png");
    image2->set_position({130.f, 205.f});
    image2->set_anchor({0., 0.});
    image2->set_rotation(30.f);

    image2->set_touch_enabled(true);
    image2->set_swallow_touches(true);
    image2->set_touched_caller([](glm::vec2 touch_position, Base_Node* image, void* global_data) {
        std::cout << "Hi there, pressed me 2! " << touch_position.x << ", " << touch_position.y << std::endl;
    });

    progression =
        Progression_Node::make("res/meow_meow/AddonBoxNumber.png", {30, 10, 74, 31}, {200, 40}, {20, 160, 20}, {6, 8});
    progression->set_position({120.f, 40.f});
    progression->do_action(Action::sequence(Action::rotate_by(3, 360, Action_Ease::SINE_OUT))->repeat_forever());

    btn = Button_Node::make(
        "res/meow_meow/BtnGreen.png",
        "START!",
        [](Button_Node* btn, void* global_data) { std::cout << "Hi pressed me 3!" << std::endl; },
        {180, 70},
        {30, 20, 280, 82},
        {20, 20, 20},
        "res/fonts/default.otf",
        28
    );
    btn->set_position({400.f, 240.f});

    layer->add_child(sub_layer);
    layer->add_child(animation);
    layer->add_child(animation_2);
    layer->add_child(label);
    this->add_child(ui);
    this->add_child(image2);
    this->add_child(layer);
    this->add_child(image);
    this->add_child(btn);
    sub_layer->add_child(progression);
}

Test_Scene::~Test_Scene() {}

void Test_Scene::fix_update(float delta_time, void* global_data) {
    if (IsKeyPressed(KEY_A)) {
        animation->stop_action(5);
        animation->do_action(
            Action::spawn(
                Action::move_by(0.2 /*delta_time=*/, {-50.f, 0}, Action_Ease::LINEAR),
                Action::sequence(
                    Action::scale_to(0.1 /*delta_time=*/, {1.2f, 1.2f}, Action_Ease::LINEAR),
                    Action::scale_to(0.1 /*delta_time=*/, {1.f, 1.f}, Action_Ease::LINEAR)
                ),
                Action::sequence(
                    Action::fade_to(0.1 /*delta_time=*/, Math::random_int(125, 220), Action_Ease::LINEAR),
                    Action::fade_in(0.1 /*delta_time=*/, Action_Ease::LINEAR)
                )
            ),
            5
        );
        animation->set_flipped_x(true);
    } else if (IsKeyPressed(KEY_D)) {
        animation->stop_action(5);
        animation->do_action(Action::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
        animation->set_flipped_x(false);
    } else if (IsKeyPressed(KEY_W)) {
        animation->stop_action(5);
        animation->do_action(
            Action::sequence(
                Action::delay(0.2),
                Action::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR),
                Action::call_func([](Base_Node* target, void* global_data) {
                    std::cout << "Move up finish!" << std::endl;
                })
            ),
            5
        );
    } else if (IsKeyPressed(KEY_S)) {
        animation->stop_action(5);
        animation->do_action(
            Action::sequence(
                Action::delay(0.5),
                Action::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR),
                Action::call_func([](Base_Node* target, void* global_data) {
                    std::cout << "Move down finish!" << std::endl;
                })
            ),
            5
        );
    } else if (IsKeyPressed(KEY_F)) {
        animation->stop_action(5);
        float duration = 0.5;
        glm::vec2 start_position = animation->get_position();
        glm::vec2 end_position =
            start_position + glm::vec2{-Math::random_float(100, 150), -Math::random_float(50, 100)};
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position,
            end_position,
            Math::random_float(100, 200),
            Math::random_float(0.5),
            Math::random_float() >= 0.5 ? -1 : 1
        );
        animation->do_action(
            Action::sequence(
                Action::delay(0.1),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_by(duration / 2, -5, Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    ),
                    Action::bezier_to(duration, middle_position, end_position, Action_Ease::SINE_OUT)
                )
            ),
            5
        );
    } else if (IsKeyPressed(KEY_G)) {
        animation->stop_action(5);
        float duration = 0.5;
        glm::vec2 start_position = animation->get_position();
        glm::vec2 end_position = start_position + glm::vec2{Math::random_float(100, 150), Math::random_float(50, 100)};
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position,
            end_position,
            Math::random_float(100, 200),
            Math::random_float(0.5),
            Math::random_float() >= 0.5 ? -1 : 1
        );
        animation->do_action(
            Action::sequence(
                Action::delay(0.1),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_by(duration / 2, -5, Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    ),
                    Action::bezier_to(duration, middle_position, end_position, Action_Ease::SINE_OUT)
                )
            ),
            5
        );
    }

    if (IsKeyPressed(KEY_LEFT)) {
        if (animation_2->get_parent() == nullptr) {
            layer->add_child(animation_2);
        }
        animation_2->stop_action(5);
        animation_2->do_action(
            Action::spawn(
                Action::move_by(0.2 /*delta_time=*/, {-50.f, 0}, Action_Ease::LINEAR),
                Action::sequence(
                    Action::scale_to(0.1 /*delta_time=*/, {1.2f, 1.2f}, Action_Ease::LINEAR),
                    Action::scale_to(0.1 /*delta_time=*/, {1.f, 1.f}, Action_Ease::LINEAR)
                ),
                Action::sequence(
                    Action::fade_to(0.1 /*delta_time=*/, Math::random_int(125, 220), Action_Ease::LINEAR),
                    Action::fade_in(0.1 /*delta_time=*/, Action_Ease::LINEAR)
                )
            ),
            5
        );
        animation_2->set_flipped_x(true);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        if (animation_2->get_parent() == nullptr) {
            layer->add_child(animation_2);
        }
        animation_2->stop_action(5);
        animation_2->do_action(Action::move_by(0.2, {50.f, 0}, Action_Ease::LINEAR), 5);
        animation_2->set_flipped_x(false);
    } else if (IsKeyPressed(KEY_UP)) {
        if (animation_2->get_parent() == nullptr) {
            layer->add_child(animation_2);
        }
        animation_2->stop_action(5);
        animation_2->do_action(Action::move_by(0.2, {0, 50.f}, Action_Ease::LINEAR), 5);
    } else if (IsKeyPressed(KEY_DOWN)) {
        if (animation_2->get_parent() == nullptr) {
            layer->add_child(animation_2);
        }
        animation_2->stop_action(5);
        animation_2->do_action(Action::move_by(0.2, {0, -50.f}, Action_Ease::LINEAR), 5);
    }
}