#include <actions.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/ui/battle_wave_progression_ui.h>
#include <utils.h>

#include <algorithm>

namespace Meow_Meow {
    Battle_Wave_Progression_UI::Battle_Wave_Progression_UI() {
        this->init_image_enemy();
        this->init_wave_progression();
    }

    Battle_Wave_Progression_UI::~Battle_Wave_Progression_UI() {}

    void Battle_Wave_Progression_UI::init_image_enemy() {
        this->image_enemy = new Image_Node(PATH_IMAGE_ENEMY);
        this->image_enemy->set_anchor({0.f, 0.f});
        this->image_enemy->set_scale(ORIGIN_IMAGE_ENEMY_SCALE);
        this->add_child(this->image_enemy);
    }

    void Battle_Wave_Progression_UI::init_wave_progression() {
        this->wave_progression = Progression_Node::make(
            PATH_IMAGE_PROGRESSION_WAVE,
            true,
            PROGRESSION_CAP_INSETS,
            PROGRESSION_RENDERER_SIZE,
            PROGRESSION_WAVE_COLOR,
            PROGRESSION_INNER_PADDING,
            PROGRESSION_INNER_DELTA_POSITION
        );
        this->wave_progression->set_anchor({0.f, 0.5f});
        Custom::Size image_size = this->image_enemy->get_scaled_size();
        this->wave_progression->set_position({image_size.width + GAP_WIDTH_UI, image_size.height / 2});
        this->add_child(this->wave_progression);
    }

    void Battle_Wave_Progression_UI::init_image_wave_arrows(void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        int current_battle_level = data->get_current_battle_level();
        const Battle_Config& battle_config = data->get_config().get_battle_config_at(current_battle_level);
        glm::vec2 progression_position = this->wave_progression->get_position();
        Custom::Size progression_size = this->wave_progression->get_renderer_size();
        float total_generated_duration = battle_config.get_total_duration_generate();
        float total_current_duration = 0;
        for (int i = 0; i < battle_config.get_number_wave(); i++) {
            Image_Node* ui = new Image_Node(PATH_IMAGE_ARROW);
            total_current_duration += battle_config.get_duration_generate_enemy_at(i);
            float delta_position_x = total_current_duration / total_generated_duration * progression_size.width * 0.8f;
            ui->set_position(
                {progression_position.x + delta_position_x + DELTA_POSITION_ARROW_X,
                 progression_position.y + progression_size.height + GAP_HEIGHT_ARROW_WITH_PROGRESSION}
            );
            ui->set_scale(ORIGIN_IMAGE_ARROW_SCALE);
            Utils::save_transform_origin(ui);
            this->image_wave_arrows.push_back(ui);
            this->add_child(ui);
        }
    }

    void Battle_Wave_Progression_UI::on_idle_arrow_at_current_wave() {
        int arrow_index = std::max(0, std::min(this->current_wave - 1, (int)this->image_wave_arrows.size() - 1));
        for (int i = 0; i < this->image_wave_arrows.size(); i++) {
            Node* arrow = this->image_wave_arrows[i];
            arrow->stop_action(ACTION_IDLE_ARROW_TAG);
            Utils::reset_to_origin(arrow);
            arrow->set_opacity(255);
            arrow->set_color({255, 255, 255});
            if (i != arrow_index) {
                arrow->set_opacity(OPACITY_DISABLE_ARROW);
                arrow->set_color(COLOR_DISABLE_ARROW);
            } else {
                float duration = 1.f;
                Custom::Transform origin = Utils::get_transform_origin(arrow);
                Base_Action* action_idle = Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, glm::vec2{1.1f, 1.1f} * origin.scale, Action_Ease::SINE_OUT),
                        Action::scale_to(duration / 2, origin.scale, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::move_to(duration / 2, glm::vec2{0, 5.f} + origin.position, Action_Ease::SINE_OUT),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::fade_to(duration / 2, 235, Action_Ease::SINE_OUT),
                        Action::fade_in(duration / 2, Action_Ease::SINE_IN)
                    )
                );
                arrow->do_action(action_idle->repeat_forever(), ACTION_IDLE_ARROW_TAG);
            }
        }
    }

    void Battle_Wave_Progression_UI::attach(void* global_data) {
        this->init_image_wave_arrows(global_data);
    }

    void Battle_Wave_Progression_UI::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        int current_battle_level = data->get_current_battle_level();
        const Battle_Config& battle_config = data->get_config().get_battle_config_at(current_battle_level);
        this->wave_progression->set_percent(
            data->get_current_battle_duration() / battle_config.get_total_duration_generate() * 100
        );
        if (this->current_wave != data->get_current_battle_wave()) {
            this->current_wave = data->get_current_battle_wave();
            this->on_idle_arrow_at_current_wave();
        }
    }

    float Battle_Wave_Progression_UI::get_ui_width() {
        return (this->image_enemy->get_scaled_size().width + GAP_WIDTH_UI +
                this->wave_progression->get_renderer_size().width) *
               this->get_scale_x();
    }
}  // namespace Meow_Meow