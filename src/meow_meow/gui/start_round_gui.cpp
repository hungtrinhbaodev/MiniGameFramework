#include <actions.h>
#include <meow_meow/data/global_data.h>
#include <meow_meow/gui/start_round_gui.h>
#include <utils.h>
#include <wrapper.h>

namespace Meow_Meow {
    Start_Round_GUI::Start_Round_GUI() {
        this->set_content_size({Libs_Wrapper::get_screen_width(), Libs_Wrapper::get_screen_height()});
        this->set_enable_layer_color(true);
        this->set_opacity(ORIGIN_BACKGROUND_OPACITY);
        this->set_layer_color(BACKGROUND_COLOR);
        this->init_label_round();
        this->set_visible(false);
        Utils::save_transform_origin(this);
    }

    Start_Round_GUI::~Start_Round_GUI() {}

    void Start_Round_GUI::show() {
        float duration = this->effect_run_label_round(0.f);
        Utils::reset_to_origin(this);
        this->set_opacity(0);
        float duration_fade_in = 0.35;
        Custom::Transform origin = Utils::get_transform_origin(this);
        this->do_action(
            Action::sequence(
                Action::show(),
                Action::spawn(
                    Action::delay(duration), Action::fade_to(duration_fade_in, origin.opacity, Action_Ease::SINE_OUT)
                ),
                Action::fade_out(duration_fade_in),
                Action::call_func([](Base_Node* target, void*) {
                    Utils::reset_to_origin(target);
                    target->set_visible(false);
                })
            )
        );
    }

    void Start_Round_GUI::fix_update(float delta_time, void* global_data) {
        Global_Data* data = reinterpret_cast<Global_Data*>(global_data);
        this->label_start_round->set_text("ROUND " + std::to_string(data->get_current_battle_wave()) + "!");
    }

    void Start_Round_GUI::init_label_round() {
        this->label_start_round = new Label_Node("", "", LABEL_FONT_SIZE);
        Custom::Size layer_size = this->get_content_size();
        this->label_start_round->set_position(ORIGIN_LABEL_RATIO_POSITION * layer_size.to_vec2());
        this->label_start_round->set_anchor({0.5f, 0.5f});
        this->label_start_round->set_color(LABEL_COLOR);
        this->add_child(this->label_start_round);
        Utils::save_transform_origin(this->label_start_round);
    }

    float Start_Round_GUI::effect_run_label_round(float delay) {
        this->stop_action(ACTION_SHOW_LABEL_TAG);
        float duration_show = 0.35f, duration_hide = 0.35f, duration_watch = 0.65f;
        Custom::Transform origin = Utils::get_transform_origin(this->label_start_round);
        this->label_start_round->set_position(origin.position + glm::vec2(-200, 0));
        this->label_start_round->set_opacity(0);
        this->label_start_round->do_action(
            Action::sequence(
                Action::delay(delay),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration_show / 2, origin.position + glm::vec2(30, 0), Action_Ease::SINE_OUT),
                        Action::move_to(duration_show / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration_show / 2, {0.75f, 1.35f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration_show / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::fade_in(duration_show * 0.35, Action_Ease::SINE_OUT)
                ),
                Action::delay(duration_watch),
                Action::spawn(
                    Action::sequence(
                        Action::move_to(duration_hide / 2, origin.position + glm::vec2(-30, 0), Action_Ease::SINE_OUT),
                        Action::move_to(duration_hide / 2, origin.position + glm::vec2(200, 0), Action_Ease::SINE_IN)
                    ),
                    Action::sequence(
                        Action::scale_to(duration_hide / 2, {0.75f, 1.25f}, Action_Ease::SINE_OUT),
                        Action::scale_to(duration_hide / 2, {1.f, 1.f}, Action_Ease::SINE_IN)
                    ),
                    Action::fade_out(2 * duration_hide, Action_Ease::SINE_OUT)
                )
            )
        );
        return duration_show + duration_hide + duration_watch;
    }
}  // namespace Meow_Meow