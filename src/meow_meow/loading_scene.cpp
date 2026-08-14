#include <actions.h>
#include <director.h>
#include <math_custom.h>
#include <meow_meow/loading_scene.h>
#include <test/test_scene.h>

namespace Meow_Meow {
    Loading_Scene::Loading_Scene() {
        init_bg();
        init_logo();
        init_loading();
    }

    Loading_Scene::~Loading_Scene() {}

    void Loading_Scene::init_loading() {
        Custom::Size screen_size = get_content_size();
        loading = Progression_Node::make(
            "res/meow_meow/AddonBoxNumber.png",
            CAP_INSETS_LOADING_BAR,
            RENDERER_SIZE_LOADING_BAR,
            COLOR_PROGRESSION_BAR,
            DELTA_POSITION_INNER_PROGRESSION
        );
        loading->set_position({screen_size.width / 2, POSITION_Y_PRORESSION});
        this->add_child(loading);
    }

    void Loading_Scene::init_logo() {
        Custom::Size screen_size = get_content_size();
        logo = new Image_Node("res/meow_meow/logo_game.png");
        logo->set_position({screen_size.width / 2, screen_size.height * RATIO_Y_LOGO});
        logo->set_scale(ORIGIN_SCALE_LOGO);
        this->add_child(logo);
    }

    void Loading_Scene::init_bg() {
        Custom::Size screen_size = get_content_size();
        bg = new Image_Node("res/meow_meow/bg_loading_scene.png");
        Custom::Size bg_size = bg->get_content_size();
        bg->set_position({screen_size.width / 2, screen_size.height / 2});
        float scale = std::max(screen_size.width / bg_size.width, screen_size.height / bg_size.height);
        bg->set_scale({scale, scale});
        this->add_child(bg);
    }

    void Loading_Scene::attach() {
        loading->do_action(
            Action::sequence(
                Action::progress_to(Math::random_float(3, 4), 100, Action_Ease::SINE_IN),
                Action::call_func([](Base_Node* base_node, void* global_data) {
                    Director::get()->change_scene(new Test_Scene(), nullptr);
                })
            )
        );
        float duration = 0.35;
        Base_Action* logo_action = Action::sequence(
            Action::scale_to(duration / 2, glm::vec2({1.15, 1.15}) * ORIGIN_SCALE_LOGO, Action_Ease::SINE_OUT),
            Action::scale_to(duration / 2, ORIGIN_SCALE_LOGO, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, -5, Action_Ease::SINE_IN),
            Action::rotate_by(duration / 2, 5, Action_Ease::SINE_OUT)
        );
        logo->do_action(logo_action->repeat_forever());
    }

}  // namespace Meow_Meow