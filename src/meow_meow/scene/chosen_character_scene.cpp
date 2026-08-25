#include <actions.h>
#include <director.h>
#include <math.h>
#include <math_custom.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/scene/chosen_character_scene.h>
#include <meow_meow/utils.h>
#include <test/test_scene.h>
#include <utils.h>

namespace Meow_Meow {

    Chosen_Character_Scene::Chosen_Character_Scene() {
        init_bg();
        init_lights();
        init_logo();
        init_button_start();
        init_btn_side(btn_left, true);
        init_btn_side(btn_right, false);
    }

    Chosen_Character_Scene::~Chosen_Character_Scene() {}

    void Chosen_Character_Scene::init_bg() {
        Custom::Size screen_size = get_content_size();
        bg = new Image_Node("res/meow_meow/bg_loading_scene.png");
        fix_background_to_scene(bg, screen_size);
        bg->set_color(ORIGIN_BG_COLOR);
        this->add_child(bg);
        this->track_layer_background(bg);
        Utils::save_transform_origin(bg);
    }

    void Chosen_Character_Scene::init_logo() {
        Custom::Size screen_size = get_content_size();
        logo = new Image_Node("res/meow_meow/logo_game.png");
        logo->set_position(glm::vec2(screen_size.width, screen_size.height) * ORIGIN_RATIO_POSITION_LOGO);
        logo->set_scale(ORIGIN_SCALE_LOGO);
        logo->set_z_order(1);
        Utils::save_transform_origin(logo);
        this->add_child(logo);
    }

    void Chosen_Character_Scene::init_lights() {
        Custom::Size screen_size = get_content_size();
        for (int i = 0; i < NUMBER_LIGHT; i++) {
            bool is_left = i % 2 == 0;
            int rotation = ORIGIN_LIGHT_RIGHT_ROTATION;
            glm::vec2 ratio_position = ORIGIN_LIGHT_RIGHT_RATIO_POSITION;
            if (is_left) {
                rotation = ORIGIN_LIGHT_LEFT_ROTATION;
                ratio_position = ORIGIN_LIGHT_LEFT_RATIO_POSITION;
            }
            Image_Node* light = new Image_Node("res/meow_meow/light.png");
            light->set_anchor({0.5, 1});
            light->set_position(glm::vec2{screen_size.width, screen_size.height} * ratio_position);
            light->set_rotation(rotation);
            light->set_opacity(ORIGIN_LIGHT_OPACITY);
            light->set_scale(ORIGIN_LIGHT_SCALE);
            this->add_child(light);
            this->lights.push_back(light);
            Utils::save_transform_origin(light);
        }
    }

    void Chosen_Character_Scene::init_btn_side(Animation_Node*& btn, bool is_left) {
        Custom::Size screen_size = get_content_size();
        btn = new Animation_Node();
        btn->make_animation("IDLE", "res/meow_meow/ui/Up", 4, 0.35);
        btn->make_animation("PAUSE", "res/meow_meow/ui/Up", 1, 0);
        btn->set_touch_enabled(true);
        btn->set_swallow_touches(true);
        btn->set_touched_caller([this, is_left](glm::vec2, Base_Node* target, void* global_data) {
            this->on_change_character(this->current_character_index + (is_left ? -1 : 1));
            Node* btn = reinterpret_cast<Node*>(target);
            btn->stop_all_action();
            btn->set_scale({1, 1});
            float duration = 0.15;
            btn->do_action(
                Action::sequence(
                    Action::scale_to(duration / 2, {1.2, 1.2}, Action_Ease::SINE_OUT),
                    Action::scale_to(duration / 2, {1, 1}, Action_Ease::SINE_IN)
                )
            );
        });
        float rotation = 90;
        glm::vec2 ratio = ORIGIN_RATIO_POSITION_BUTTON_RIGHT;
        btn->play_animation("IDLE", 1.f);
        if (is_left) {
            ratio = ORIGIN_RATIO_POSITION_BUTTON_LEFT;
            rotation = -90;
            btn->play_animation("PAUSE", 1);
            btn->set_opacity(DISABLE_OPACITY_BUTTON_SIDES);
            btn->set_color(DISABLE_COLOR_BUTTON_SIDES);
        }
        btn->set_position(glm::vec2(screen_size.width, screen_size.height) * ratio);
        btn->set_rotation(rotation);
        this->add_child(btn);
    }

    void Chosen_Character_Scene::init_button_start() {
        Custom::Size screen_size = get_content_size();
        btn_start = Button_Node::make(
            "res/meow_meow/BtnGreen.png",
            "START!",
            [](Button_Node* btn, void* global_data) { Director::get()->change_scene(new Test_Scene(), nullptr); },
            {180, 70},
            {30, 20, 280, 82},
            {20, 20, 20},
            "res/fonts/default.otf",
            28
        );
        btn_start->set_position({screen_size.width / 2, ORIGIN_POSITION_Y_BUTTON_START});
        this->add_child(btn_start);
    }

    void Chosen_Character_Scene::init_characters(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        auto config = data->get_config();
        std::vector<int> character_ids = config.get_character_ids();
        for (int i = 0; i < character_ids.size(); i++) {
            int character_id = character_ids[i];
            Chosen_Character_UI* ui = new Chosen_Character_UI(character_id);
            ui->set_position(ORIGIN_POSITION_CHARACTER);
            ui->set_visible(i == current_character_index);
            this->add_child(ui);
            characters.push_back(ui);
        }
    }

    void Chosen_Character_Scene::update_btn_side_state(Animation_Node* btn, bool is_disable) {
        btn->play_animation(is_disable ? "PAUSE" : "IDLE");
        btn->set_color(is_disable ? DISABLE_COLOR_BUTTON_SIDES : Custom::Color{255, 255, 255});
        btn->set_opacity(is_disable ? DISABLE_OPACITY_BUTTON_SIDES : 255);
    }

    float Chosen_Character_Scene::effect_shake_light_when_change_character(float delay) {
        float duration = 0.65;
        for (Image_Node* light : this->lights) {
            Custom::Transform origin = Utils::get_transform_origin(light);
            float rotation = origin.rotation;
            float rotation_sign = rotation / std::abs(rotation);
            glm::vec2 scale = origin.scale;
            unsigned char opacity = origin.opacity;
            light->stop_all_action();
            light->do_action(
                Action::sequence(
                    Action::delay(delay),
                    Action::spawn(
                        Action::sequence(
                            Action::rotate_to(duration * 0.5, rotation - rotation_sign * 20, Action_Ease::SINE_IN),
                            Action::rotate_to(duration * 0.5, rotation, Action_Ease::SINE_OUT)
                        ),
                        Action::sequence(
                            Action::scale_to(duration * 0.5, scale * glm::vec2{1.1, 1}, Action_Ease::SINE_IN),
                            Action::scale_to(duration * 0.5, scale, Action_Ease::SINE_OUT)
                        ),
                        Action::sequence(
                            Action::fade_out(duration * 0.8, Action_Ease::SINE_IN),
                            Action::delay(duration * 0.2),
                            Action::fade_to(duration * 0.5, opacity, Action_Ease::SINE_OUT)
                        )
                    ),
                    Action::call_func([this](Base_Node* target, void* global_data) {
                        Node* light = reinterpret_cast<Node*>(target);
                        this->effect_idle_light(light);
                    })
                )
            );
        }
        return duration;
    }

    void Chosen_Character_Scene::on_change_character(int next_character) {
        next_character = std::max(0, std::min(next_character, (int)this->characters.size() - 1));
        if (this->current_character_index == next_character) {
            return;
        }
        this->current_character_index = next_character;
        for (int i = 0; i < this->characters.size(); i++) {
            Chosen_Character_UI* ui = this->characters[i];
            ui->set_visible(i == this->current_character_index);
        }
        bool is_most_left = this->current_character_index <= 0;
        bool is_most_right = this->current_character_index >= this->characters.size() - 1;
        this->update_btn_side_state(this->btn_left, is_most_left);
        this->update_btn_side_state(this->btn_right, is_most_right);
        this->effect_shake_light_when_change_character(0);
    }

    float Chosen_Character_Scene::effect_show_lights(float delay) {
        float duration = 0.65;
        for (Image_Node* light : lights) {
            Custom::Transform origin = Utils::get_transform_origin(light);
            light->stop_all_action();
            light->set_scale({0, 0});
            light->set_rotation(-origin.rotation);
            light->set_opacity(0);
            int rotation_sign = origin.rotation / std::abs(origin.rotation);
            unsigned char origin_opacity = origin.opacity;
            glm::vec2 origin_scale = origin.scale;
            light->do_action(
                Action::sequence(
                    Action::delay(delay),
                    Action::spawn(
                        Action::sequence(
                            Action::scale_to(duration / 2, glm::vec2(1.05, 1.1) * origin.scale, Action_Ease::SINE_OUT),
                            Action::scale_to(duration / 2, glm::vec2(1, 1) * origin.scale, Action_Ease::SINE_IN)
                        ),
                        Action::sequence(
                            Action::rotate_to(
                                duration * 0.65, origin.rotation + rotation_sign * 5, Action_Ease::SINE_OUT
                            ),
                            Action::rotate_to(duration * 0.35, origin.rotation, Action_Ease::SINE_IN)
                        ),
                        Action::sequence(
                            Action::fade_to(duration * 0.65, 140), Action::fade_to(duration * 0.35, origin_opacity)
                        )
                    ),
                    Action::call_func([this](Base_Node* target, void* global_data) {
                        Node* light = reinterpret_cast<Node*>(target);
                        this->effect_idle_light(light);
                    })
                )
            );
        }
        return duration;
    }

    void Chosen_Character_Scene::effect_idle_light(Node* light) {
        Custom::Transform origin = Utils::get_transform_origin(light);
        unsigned char origin_opacity = origin.opacity;
        glm::vec2 origin_scale = origin.scale;
        Base_Action* action = Action::spawn(
            Action::sequence(
                Action::fade_to(0.5, std::min((unsigned char)(origin_opacity * 1.2), (unsigned char)255)),
                Action::fade_to(0.5, origin_opacity)
            ),
            Action::sequence(
                Action::scale_to(0.75, origin_scale * glm::vec2(1.1, 1.1), Action_Ease::SINE_OUT),
                Action::scale_to(0.75, origin_scale * glm::vec2(1, 1), Action_Ease::SINE_IN)
            )
        );
        light->do_action(action->repeat_forever());
    }

    float Chosen_Character_Scene::effect_show_logo(float delay) {
        float duration = 0.55;
        Custom::Size size = get_content_size();
        Custom::Transform origin = Utils::get_transform_origin(logo);
        glm::vec2 start_position{size.width / 2, size.height / 2};
        glm::vec2 start_scale{0.3, 0.3};
        glm::vec2 end_position = origin.position;
        glm::vec2 middle_position = Math::get_middle_bezier_point(
            start_position, end_position, Math::random_float(100, 200), Math::random_float(), -1
        );
        logo->stop_all_action();
        logo->set_position(start_position);
        logo->set_scale(start_scale);
        logo->set_rotation(Math::random_float(-15, 15));
        logo->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::bezier_to(duration, middle_position, end_position, Action_Ease::SINE_IN),
                    Action::sequence(
                        Action::scale_to(duration * 0.5, glm::vec2{1.1, 1.1} * start_scale, Action_Ease::SINE_OUT),
                        Action::scale_to(duration * 0.5, origin.scale, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_to(duration * 0.5, Math::random_float(15, 30), Action_Ease::SINE_OUT),
                        Action::rotate_to(duration * 0.35, 0, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(Action::fade_to(duration * 0.75, 200), Action::fade_in(duration * 0.75))
                ),
                Action::call_func([this](Base_Node* target, void* global_data) {
                    run_action_idle_logo(this->logo, ORIGIN_SCALE_LOGO);
                })
            ),
            -1,
            true
        );
        return duration;
    }

    float Chosen_Character_Scene::effect_blur_bg(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(bg);
        float duration = 1;
        bg->set_opacity(255);
        bg->do_action(
            Action::sequence(Action::delay(delay), Action::fade_to(duration, origin.opacity, Action_Ease::SINE_IN))
        );
        return duration;
    }

    void Chosen_Character_Scene::effect_show_scene() {
        float total_time = 0;
        total_time += std::max(
            std::max(effect_show_lights(total_time), effect_show_logo(total_time)), effect_blur_bg(total_time)
        );
    }

    void Chosen_Character_Scene::attach(void* global_data) {
        init_characters(global_data);
        effect_show_scene();
    }

}  // namespace Meow_Meow