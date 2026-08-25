#include <meow_meow/data/global_data.h>
#include <meow_meow/ui/chosen_character_ui.h>

namespace Meow_Meow {
    Chosen_Character_UI::Chosen_Character_UI() {
        this->set_anchor({0.5f, 0.f});
        this->init_hall();
        this->init_character_name();
        this->init_character();
    }

    Chosen_Character_UI::Chosen_Character_UI(int character_id) : Chosen_Character_UI() {
        this->character_id = character_id;
    }

    Chosen_Character_UI::~Chosen_Character_UI() {}

    void Chosen_Character_UI::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        auto& config = data->get_config();
        this->character->set_character_id(this->character_id);
        this->character->set_character_level(1);
        this->character_name->set_text(config.get_character_name(this->character_id));
        this->character->play_animation("IDLE", 1.f);
    }

    void Chosen_Character_UI::init_hall() {
        hall = new Image_Node("res/meow_meow/hall2.png");
        hall->set_color(HALL_COLOR);
        hall->set_scale(ORIGIN_SCALE_HALL);
        this->add_child(hall);
    }

    void Chosen_Character_UI::init_character() {
        character = new Character_Animation();
        character->set_scale(CHARACTER_SCALE);
        character->set_position({ORIGIN_CHACTER_POSITION});
        this->add_child(character);
    }

    void Chosen_Character_UI::init_character_name() {
        character_name = new Label_Node("", "fonts/default.otf", 32);
        character_name->set_position(ORIGIN_CHARACTER_NAME_POSITION);
        character_name->set_anchor({0.5, 0.5});
        character_name->set_color(CHARACTER_NAME_COLOR);
        character_name->set_opacity(CHARACTER_NAME_OPACITY);
        this->add_child(character_name);
    }

}  // namespace Meow_Meow