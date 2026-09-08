#include <actions.h>
#include <math_custom.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/gui/win_game_gui.h>
#include <utils.h>
#include <wrapper.h>

namespace Meow_Meow {
    Win_Game_GUI::Win_Game_GUI() {
        this->set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
        this->init_image_win_game();
        this->init_character_ui();
        this->init_btn_back();
        this->set_enable_layer_color(true);
        this->set_layer_color(BACKGROUND_COLOR);
        this->set_opacity(ORIGIN_BACKGROUND_OPACITY);
    }

    Win_Game_GUI::~Win_Game_GUI() {}

    void Win_Game_GUI::init_image_win_game() {
        this->image_win = new Image_Node(PATH_IMAGE_WIN_GAME);
        this->image_win->set_scale(ORIGIN_IMAGE_WIN_GAME_SCALE);
        Custom::Size layer_size = this->get_content_size();
        this->image_win->set_position(layer_size.to_vec2() * ORIGIN_RATIO_POSITION_IMAGE_WIN);
        this->add_child(this->image_win);
        Utils::save_transform_origin(this->image_win);
    }

    void Win_Game_GUI::init_character_ui() {
        this->character = new Chosen_Character_UI();
        this->character->set_hide_label_name(true);
        Custom::Size layer_size = this->get_content_size();
        this->character->set_position(layer_size.to_vec2() * ORIGIN_RATIO_POSITION_CHARACTER);
        this->add_child(this->character);
        Utils::save_transform_origin(this->character);
    }

    void Win_Game_GUI::init_btn_back() {
        Custom::Size screen_size = get_content_size();
        this->btn_back = Button_Node::make(
            "res/meow_meow/BtnGreen.png",
            "GREATE!",
            [this](Button_Node* btn, void* global_data) {

            },
            {180, 70},
            {30, 20, 280, 82},
            {20, 20, 20},
            "res/fonts/default.otf",
            28
        );
        Custom::Size layer_size = this->get_content_size();
        this->btn_back->set_position(layer_size.to_vec2() * ORIGIN_RATIO_POSITION_BUTTON_BACK);
        this->add_child(this->btn_back);
        Utils::save_transform_origin(this->btn_back);
    }

    void Win_Game_GUI::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        this->character->set_character_id(data->get_player_data().get_character_animation_id());
        this->character->set_character_level(data->get_player_data().get_character_level());
        float total_time = 0.f;
        total_time += this->effect_show_image_win(total_time);
        total_time += this->effect_show_character(total_time) * 0.85;
        this->effect_show_btn_back(total_time);
    }

    float Win_Game_GUI::effect_show_image_win(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(this->image_win);
        this->image_win->set_opacity(0);
        this->image_win->set_position(origin.position + glm::vec2(Math::random_float(-100, 100), -100));
        this->image_win->set_scale({0.2f, 0.2f});
        this->image_win->set_visible(false);
        float duration = 0.55f;
        this->image_win->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(
                            duration / 2,
                            origin.position +
                                glm::vec2(Math::random_float(-100, 100), 100 + Math::random_float(0, 100)),
                            Action_Ease::SINE_OUT
                        ),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration / 2, glm::vec2{0.75f, 0.75f} * origin.scale, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, origin.scale, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::rotate_to(duration / 2, Math::random_float(-10, 10), Action_Ease::SINE_OUT),
                        Action::rotate_to(duration / 2, 0, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration * 0.35)
                )
            )
        );
        return duration;
    }

    float Win_Game_GUI::effect_show_character(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(this->character);
        this->character->set_opacity(0);
        this->character->set_position(origin.position + glm::vec2{0, 200});
        this->character->set_scale({0.f, 0.f});
        this->character->set_visible(false);
        float duration = 0.4f;
        this->character->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration / 2, origin.position + glm::vec2(0, -30), Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.15f, 0.85f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, origin.scale, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration * 0.35)
                )
            )
        );
        return duration;
    }

    float Win_Game_GUI::effect_show_btn_back(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(this->btn_back);
        this->btn_back->set_opacity(0);
        this->btn_back->set_position(origin.position + glm::vec2{0, -50});
        float duration = 0.35f;
        this->btn_back->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration / 2, origin.position + glm::vec2(0, 10), Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration * 0.35)
                )
            )
        );
        return duration;
    }
}  // namespace Meow_Meow