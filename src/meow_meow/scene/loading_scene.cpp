#include <actions.h>
#include <director.h>
#include <math_custom.h>
#include <meow_meow/animation/character_animation.h>
#include <meow_meow/global_data.h>
#include <meow_meow/scene/chosen_character_scene.h>
#include <meow_meow/scene/loading_scene.h>
#include <meow_meow/utils.h>
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
        fix_background_to_scene(bg, screen_size);
        this->add_child(bg);
        this->track_layer_background(bg);
    }

    void Loading_Scene::attach(void* global_data) {
        loading->do_action(
            Action::sequence(
                Action::progress_to(Math::random_float(3, 4), 100, Action_Ease::SINE_IN),
                Action::call_func([](Base_Node* base_node, void* global_data) {
                    Director::get()->change_scene(new Chosen_Character_Scene(), Global_Data::get());
                })
            )
        );
        run_action_idle_logo(logo, ORIGIN_SCALE_LOGO);
    }

}  // namespace Meow_Meow