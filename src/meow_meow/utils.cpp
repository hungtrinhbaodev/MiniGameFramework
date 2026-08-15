#include <actions.h>
#include <meow_meow/utils.h>

namespace Meow_Meow {
    void fix_background_to_scene(Image_Node* bg, Custom::Size screen_size) {
        Custom::Size bg_size = bg->get_content_size();
        bg->set_position({screen_size.width / 2, screen_size.height / 2});
        float scale = std::max(screen_size.width / bg_size.width, screen_size.height / bg_size.height);
        bg->set_scale({scale, scale});
    }

    void run_action_idle_logo(Node* logo, glm::vec2 origin_scale) {
        float duration = 0.35;
        Base_Action* logo_action = Action::sequence(
            Action::scale_to(duration / 2, glm::vec2({1.15, 1.15}) * origin_scale, Action_Ease::SINE_OUT),
            Action::scale_to(duration / 2, origin_scale, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, -5, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, 5, Action_Ease::SINE_OUT)
        );
        logo->do_action(logo_action->repeat_forever());
    }
}  // namespace Meow_Meow