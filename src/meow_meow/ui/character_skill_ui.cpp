#include <meow_meow/data/global_data.h>
#include <meow_meow/ui/character_skill_ui.h>

namespace Meow_Meow {
    Character_Skill_UI::Character_Skill_UI() {}

    Character_Skill_UI::Character_Skill_UI(std::string skill_id) : Character_Skill_UI() {
        this->skill_id = skill_id;
    }

    Character_Skill_UI::~Character_Skill_UI() {}

    float Character_Skill_UI::get_ui_height() {
        if (this->progression_skill == nullptr)
            return 0.f;
        glm::vec2 image_scale = this->progression_skill->get_scale();
        Custom::Size image_size = this->progression_skill->get_renderer_size();
        glm::vec2 scale = this->get_scale();
        return image_size.height * image_scale.y * 0.85 * scale.y;
    }

    void Character_Skill_UI::attach(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_UI_Config& skill_config = data->get_config().get_character_skill_ui_config();
        const Skill_Information* skill = skill_config.find_skill(this->skill_id);
        this->set_visible(skill != nullptr);
        if (skill == nullptr) {
            return;
        }
        this->init_progression_skill(skill);
        this->init_label_skill(skill);
    }

    void Character_Skill_UI::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        const Character_Skill_UI_Config& skill_config = data->get_config().get_character_skill_ui_config();
        const Skill_Information* skill = skill_config.find_skill(this->skill_id);
        if (skill == nullptr)
            return;
        if (this->progression_skill == nullptr)
            return;
        Skill_Data* skill_data = data->get_skill_data_by(this->skill_id);
        if (skill_data == nullptr)
            return;
        this->progression_skill->set_percent(100 - skill_data->percent_countdown);
    }

    void Character_Skill_UI::init_progression_skill(const Skill_Information* skill) {
        this->progression_skill = Progression_Node::make(
            skill->skill_image_path,
            false,
            {0, 0, 0, 0},
            {-1, -1},
            INNER_PROGRESSION_COLOR,
            {0, 0},
            {0, 0},
            INNER_PROGRESSION_OPACITY
        );
        this->progression_skill->set_scale(ORIGIN_PRGRESSION_SCALE);
        this->progression_skill->set_anchor({0.f, 0.f});
        this->add_child(this->progression_skill);
    }

    void Character_Skill_UI::init_label_skill(const Skill_Information* skill) {
        this->label_skill_name = new Label_Node("", "", LABEL_SKILL_FONT_SIZE);
        this->label_skill_name->set_anchor({0.f, 0.5f});
        this->label_skill_name->set_color(LABEL_SKILL_COLOR);
        Custom::Size image_size = this->progression_skill->get_renderer_size();
        glm::vec2 image_scale = this->progression_skill->get_scale();
        this->label_skill_name->set_position({PROGRESSION_FIX_WIDTH + GAP_WIDTH_UI, PROGRESSION_FIX_HEIGHT * 0.5});
        this->add_child(this->label_skill_name);
        this->label_skill_name->set_text(skill->skill_display_name);
    }

}  // namespace Meow_Meow