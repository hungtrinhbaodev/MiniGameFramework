#include <actions.h>
#include <exercises/movement_scene.h>
#include <utils.h>

namespace Exercises {
    Movement_Scene::Movement_Scene() {
        this->init_image();
        this->init_label_key_pressed();
        this->add_key_press_listener(Custom::Key::A /**Move action from A to B in 2 duration*/);
        this->add_key_press_listener(Custom::Key::V /**Move action from B to A in 1 duration*/);

        this->add_key_press_listener(Custom::Key::C /**Scale action from 1, 1 to 1.5, 1.5 in 0.5 duration*/);
        this->add_key_press_listener(Custom::Key::D /**Scale action from 1.5, 1.5 to 1, 1 in 0.5 duration*/);

        this->add_key_press_listener(Custom::Key::W /**Rotate action forever 360 in 1 duration*/);
        this->add_key_press_listener(Custom::Key::S /**Stop action rotate forever*/);
    }

    Movement_Scene::~Movement_Scene() {}

    void Movement_Scene::attach(void* global_data) {
        this->image->set_position(START_POSITION);
    }

    void Movement_Scene::on_key_pressed(Custom::Key key, Key_Press_Detail pressed_detail, void* global_data) {
        Custom::Transform origin = Utils::get_transform_origin(this->image);
        switch (key) {
            case Custom::Key::A: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->set_scale(START_SCALE);
                this->image->set_position(START_POSITION);
                this->image->do_action(Action::move_to(DURATION_MOVE, END_POSITION), ACTION_MOVE_IMAGE_TAG);
                this->action_show_key_pressed("A");
                break;
            }
            case Custom::Key::V: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->set_scale(START_SCALE);
                this->image->set_position(END_POSITION);
                this->image->do_action(Action::move_to(DURATION_MOVE, START_POSITION), ACTION_MOVE_IMAGE_TAG);
                this->action_show_key_pressed("V");
                break;
            }
            case Custom::Key::C: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->set_position(START_POSITION);
                this->image->set_scale(START_SCALE);
                this->image->do_action(Action::scale_to(DURATION_SCALE, END_SCALE), ACTION_SCALE_IMAGE_TAG);
                this->action_show_key_pressed("C");
                break;
            }
            case Custom::Key::D: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->set_position(START_POSITION);
                this->image->set_scale(END_SCALE);
                this->image->do_action(Action::scale_to(DURATION_SCALE, START_SCALE), ACTION_SCALE_IMAGE_TAG);
                this->action_show_key_pressed("D");
                break;
            }
            case Custom::Key::W: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_ROTATION_FOREVER_IMAGE_TAG);
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->set_scale(START_SCALE);
                this->image->set_position(START_POSITION);
                this->image->do_action(
                    Action::rotate_by(DURATION_ROTATE_FOREVER, ROTATION_FOREVER_PER_SECOND)->repeat_forever(),
                    ACTION_ROTATION_FOREVER_IMAGE_TAG
                );
                this->action_show_key_pressed("W");
                break;
            }
            case Custom::Key::S: {
                if (pressed_detail.type != Key_Input_Type::PRESSED)
                    break;
                this->image->stop_action(ACTION_ROTATION_FOREVER_IMAGE_TAG);
                this->image->stop_action(ACTION_SCALE_IMAGE_TAG);
                this->image->stop_action(ACTION_MOVE_IMAGE_TAG);
                this->image->set_scale(START_SCALE);
                this->image->set_position(START_POSITION);
                this->action_show_key_pressed("S");
                break;
            }
            default: {
                break;
            }
        }
    }

    void Movement_Scene::init_image() {
        this->image = new Image_Node();
        this->image->set_image(IMAGE_PATH);
        this->add_child(this->image);
        Utils::save_transform_origin(this->image);
    }

    void Movement_Scene::init_label_key_pressed() {
        this->label_key_pressed = new Label_Node("", "", 60);
        const Custom::Size screen_size = this->get_content_size();
        this->label_key_pressed->set_position({screen_size.width / 2, screen_size.height / 2});
        this->label_key_pressed->set_opacity(0);
        this->label_key_pressed->set_visible(false);
        this->label_key_pressed->set_color(LABEL_KEY_PRESSED_COLOR);
        Utils::save_transform_origin(this->label_key_pressed);
        this->add_child(this->label_key_pressed);
    }

    void Movement_Scene::action_show_key_pressed(std::string key_pressed) {
        this->label_key_pressed->stop_action(ACTION_SHOW_LABEL_PRESSED_TAG);
        Utils::reset_to_origin(this->label_key_pressed);
        this->label_key_pressed->set_visible(false);
        Custom::Transform origin = Utils::get_transform_origin(this->label_key_pressed);
        glm::vec2 start_position = origin.position + glm::vec2{0, -30};
        this->label_key_pressed->set_position(start_position);
        this->label_key_pressed->set_text(key_pressed);
        float duration = 0.35;
        this->label_key_pressed->do_action(
            Action::sequence(
                Action::show(),
                Action::spawn(
                    Action::sequence(
                        Action::scale_to(duration / 2, {1.2f, 1.2f}, Action_Ease::SINE_OUT),
                        Action::delay(0.35),
                        Action::scale_to(duration / 2, {1.f, 1.f}, Action_Ease::SINE_OUT)
                    ),
                    Action::sequence(
                        Action::move_to(duration / 2, origin.position + glm::vec2{0, 20}, Action_Ease::SINE_OUT),
                        Action::delay(0.35),
                        Action::move_to(duration / 2, origin.position, Action_Ease::SINE_IN)
                    ),
                    Action::sequence(Action::fade_in(duration * 0.35), Action::delay(0.35), Action::fade_out(duration))
                ),
                Action::hide()
            ),
            ACTION_SHOW_LABEL_PRESSED_TAG
        );
    }
}  // namespace Exercises