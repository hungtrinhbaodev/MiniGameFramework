#include <actions.h>
#include <math_custom.h>
#include <meow_meow/gui/lose_game_gui.h>
#include <utils.h>
#include <wrapper.h>

namespace Meow_Meow {

    Lose_Game_GUI::Lose_Game_GUI() {
        this->set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
        this->init_image_lose_game();
        this->init_btn_back();
        this->set_enable_layer_color(true);
        this->set_layer_color(BACKGROUND_COLOR);
        this->set_opacity(ORIGIN_BACKGROUND_OPACITY);
    }

    Lose_Game_GUI::~Lose_Game_GUI() {}

    void Lose_Game_GUI::init_image_lose_game() {
        this->image_lose = new Image_Node(PATH_IMAGE_LOSE_GAME);
        this->image_lose->set_scale(ORIGIN_IMAGE_LOSE_GAME_SCALE);
        Custom::Size layer_size = this->get_content_size();
        this->image_lose->set_position(layer_size.to_vec2() * ORIGIN_RATIO_POSITION_IMAGE_LOSE);
        this->add_child(this->image_lose);
        Utils::save_transform_origin(this->image_lose);
    }

    void Lose_Game_GUI::init_btn_back() {
        Custom::Size screen_size = get_content_size();
        this->btn_back = Button_Node::make(
            "res/meow_meow/BtnGreen.png",
            "REVENCE!",
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

    void Lose_Game_GUI::attach(void* global_data) {
        float total_time = 0;
        total_time += this->effect_show_image_lose(total_time);
        this->effect_show_btn_back(total_time);
    }

    float Lose_Game_GUI::effect_show_image_lose(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(this->image_lose);
        this->image_lose->set_opacity(0);
        this->image_lose->set_position(origin.position + glm::vec2(0, 120));
        this->image_lose->set_scale({1.2f, 1.2f});
        this->image_lose->set_visible(false);
        float duration = 0.65f;
        this->image_lose->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration / 2, origin.position + glm::vec2(0, -50), Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration / 2, glm::vec2{0.85f, 0.85f} * origin.scale, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, origin.scale, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration * 0.35)
                )
            )
        );
        return duration;
    }

    float Lose_Game_GUI::effect_show_btn_back(float delay) {
        Custom::Transform origin = Utils::get_transform_origin(this->btn_back);
        this->btn_back->set_opacity(0);
        this->btn_back->set_position(origin.position + glm::vec2{0, -70});
        float duration = 0.55f;
        this->btn_back->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration / 2, origin.position + glm::vec2(0, 10), Action_Ease::SINE_IN),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_OUT)
                    ),
                    Action::fade_in(duration * 0.35)
                )
            )
        );
        return duration;
    }
}  // namespace Meow_Meow