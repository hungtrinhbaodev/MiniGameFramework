#include <meow_meow/config/character_skill_ui_config.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/layer/layer_stats.h>
#include <wrapper.h>

namespace Meow_Meow {
    Layer_Stats::Layer_Stats() {
        this->set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
        this->init_stats_ui();
        this->set_name("Layer_Stats");
    }

    Layer_Stats::~Layer_Stats() {}

    void Layer_Stats::attach(void* global_data) {
        this->init_skills_ui(global_data);
    }

    void Layer_Stats::fix_update(float delta_time, void* global_data) {
        this->update_skills_ui_location();
    }

    void Layer_Stats::init_stats_ui() {
        this->stats_ui = new Battle_Character_Stats_UI();
        Custom::Size ui_size = this->stats_ui->get_content_size();
        this->stats_ui->set_position({0, this->get_content_size().height - ui_size.height});
        this->add_child(this->stats_ui);
    }

    void Layer_Stats::init_skills_ui(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_UI_Config& skills_config = data->get_config().get_character_skill_ui_config();
        for (int i = 0; i < skills_config.skills.size(); i++) {
            Character_Skill_UI* ui = new Character_Skill_UI(skills_config.skills[i].skill_id);
            this->add_child(ui);
            this->skills_ui.push_back(ui);
        }
    }

    void Layer_Stats::update_skills_ui_location() {
        glm::vec2 start_postion = ORIGIN_START_SKILL_UI_POSITION;
        float gap = GAP_HEIGHT_PER_SKILL_UI;
        for (int i = 0; i < skills_ui.size(); i++) {
            Character_Skill_UI* skill_ui = skills_ui[i];
            if (!skill_ui->is_visible())
                continue;
            skill_ui->set_position(start_postion);
            start_postion.y += skill_ui->get_ui_height() + gap;
        }
    }
}  // namespace Meow_Meow