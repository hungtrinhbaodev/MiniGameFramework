#include <actions.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/ui/battle_character_stats_ui.h>
#include <wrapper.h>

namespace Meow_Meow {
    Battle_Character_Stats_UI::Battle_Character_Stats_UI() {
        this->set_content_size(LAYER_SIZE);
        this->init_box_character();
        this->init_character_stats();
        this->set_name("Battle_Character_Stats_UI");
    }

    Battle_Character_Stats_UI::~Battle_Character_Stats_UI() {}

    void Battle_Character_Stats_UI::init_box_character() {
        this->background_character = new Image_UI_Node();
        this->background_character->set_image(PATH_BACKGROUND_BOX);
        this->background_character->set_enable_nine_scale(true);
        this->background_character->set_cap_insets(CAP_INSETS_BACKGROUND_BOX);
        this->background_character->set_renderer_size(SIZE_BACKGROUND_BOX);
        this->background_character->set_anchor({0.f, 0.0f});
        this->background_character->set_position(
            {LAYER_PADDING.width, (LAYER_SIZE.height - SIZE_BACKGROUND_BOX.height) / 2}
        );

        this->clipping_box = new Layer_Node();
        this->clipping_box->set_content_size(SIZE_BACKGROUND_BOX.width * 0.85, SIZE_BACKGROUND_BOX.height * 0.95);
        this->clipping_box->set_anchor({0.5, 0.5});
        this->clipping_box->set_position({SIZE_BACKGROUND_BOX.width / 2, SIZE_BACKGROUND_BOX.height / 2 + 5});
        this->clipping_box->set_clipping(true);
        this->background_character->add_child(this->clipping_box);

        this->character_animation = new Character_Animation();
        this->character_animation->set_position(ANIMATION_CHARACTER_POSITION);
        this->clipping_box->add_child(this->character_animation);

        this->add_child(this->background_character);
    }

    void Battle_Character_Stats_UI::init_character_stats() {
        float start_x = LAYER_PADDING.width + SIZE_BACKGROUND_BOX.width * 0.9 + LAYER_GAP.width;
        float start_y = LAYER_SIZE.height - LAYER_PADDING.height;
        this->health_progression = Progression_Node::make(
            PATH_PROGRESSION, CAP_INSETS_PROGRESSION, SIZE_PROGRESSION, HEALTH_COLOR_PROGRESSION, PADDING_PROGRESSION
        );
        this->health_progression->set_anchor({0.f, 0.5f});
        this->health_progression->set_position({start_x, start_y - SIZE_PROGRESSION.height / 2});
        this->add_child(this->health_progression);
        start_y -= SIZE_PROGRESSION.height + LAYER_GAP.height - 8;

        float label_padding_x = 10;
        this->label_damage = new Label_Node("", "", DAMAGE_LABEL_FONT_SIZE);
        this->label_damage->set_color(DAMAGE_LABEL_COLOR);
        Custom::Size label_size = Libs_Wrapper::text_info("1", LABEL_PATH, DAMAGE_LABEL_FONT_SIZE).size;
        this->label_damage->set_position({start_x + label_padding_x, start_y - label_size.height / 2});
        this->label_damage->set_anchor({0.f, 0.5});
        this->add_child(this->label_damage);
        start_y -= label_size.height + LAYER_GAP.height;

        this->label_level = new Label_Node("", "", LEVEL_LABEL_FONT_SIZE);
        label_size = Libs_Wrapper::text_info("1", LABEL_PATH, LEVEL_LABEL_FONT_SIZE).size;
        this->label_level->set_position({start_x + label_padding_x, start_y - label_size.height / 2});
        this->label_level->set_anchor({0.f, 0.5});
        this->label_level->set_color(LEVEL_LABEL_COLOR);
        this->add_child(this->label_level);
    }

    void Battle_Character_Stats_UI::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Player_Data& player_data = data->get_player_data();
        this->current_level = player_data.get_character_level();
        this->current_health = player_data.get_current_health();
        this->character_animation->set_character_id(player_data.get_character_animation_id());
        this->character_animation->set_character_level(this->current_level);
        this->character_animation->play_animation("IDLE");
        this->health_progression->set_percent(100);
    }

    void Battle_Character_Stats_UI::fix_update(float delta_time, void* global_data) {
        this->update_stats(global_data);
    }

    void Battle_Character_Stats_UI::update_stats(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        Player_Data& player_data = data->get_player_data();
        if (this->current_level != player_data.get_character_level()) {
            this->character_animation->set_character_level(player_data.get_character_level());
            this->character_animation->play_animation("IDLE");
            this->current_level = player_data.get_character_level();
        }
        this->label_damage->set_text("Damage: " + std::to_string((int)(player_data.get_player_damage())));
        this->label_level->set_text("Level: " + std::to_string(player_data.get_character_level()));
        if (this->current_health != player_data.get_current_health()) {
            this->current_health = player_data.get_current_health();
            this->health_progression->stop_action(ACTION_PROGRESS_HEALTH_TAG);
            float percent = player_data.get_current_health() / player_data.get_max_health() * 100;
            this->health_progression->do_action(
                Action::progress_to(PROGRESSION_DURATION, percent, Action_Ease::SINE_OUT), ACTION_PROGRESS_HEALTH_TAG
            );
        }
    }
}  // namespace Meow_Meow