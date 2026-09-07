#include <meow_meow/gui/lose_game_gui.h>
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
    }
}