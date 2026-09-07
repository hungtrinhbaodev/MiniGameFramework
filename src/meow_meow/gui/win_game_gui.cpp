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
    }

    void Win_Game_GUI::init_character_ui() {
        this->character = new Chosen_Character_UI();
        this->character->set_hide_label_name(true);
        Custom::Size layer_size = this->get_content_size();
        this->character->set_position(layer_size.to_vec2() * ORIGIN_RATIO_POSITION_CHARACTER);
        this->add_child(this->character);
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
    }

    void Win_Game_GUI::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        this->character->set_character_id(data->get_player_data().get_character_animation_id());
        this->character->set_character_level(data->get_player_data().get_character_level());
    }
}  // namespace Meow_Meow