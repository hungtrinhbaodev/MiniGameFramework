#include <image_ui_node.h>
#include <touch_component.h>
#include <wrapper.h>

#include <stdexcept>

Touch_Component::Touch_Component() {}

Touch_Component::~Touch_Component() {}

Touch_Information& Touch_Component::modify_infomation() {
    return this->touch_information;
}

void Touch_Component::enter(Base_Node* target, void* global_data) {
    this->touch_information = Touch_System::get()->request_touch_listenner();
}

void Touch_Component::exit(Base_Node* target, void* global_data) {
    Touch_System::get()->remove_touch_listener(this->touch_information.touch_id);
}

void Touch_Component::draw(Base_Node* target, int& draw_index) {
    if (!Libs_Wrapper::is_debug_mode())
        return;
    Custom::Size size;
    Node_Type type = target->get_type();
    switch (type) {
        case Node_Type::PROGRESSION:
        case Node_Type::BUTTON:
        case Node_Type::IMAGE_UI: {
            Image_UI_Node* image_ui = reinterpret_cast<Image_UI_Node*>(target);
            size = image_ui->get_renderer_size();
            break;
        }
        case Node_Type::ANIMATION:
        case Node_Type::IMAGE: {
            Image_Node* image = reinterpret_cast<Image_Node*>(target);
            size = image->get_content_size();
            break;
        }
        default: {
            throw std::runtime_error(
                std::string("Touch component error: Unsuported type of Node ") + std::to_string(type) + "!"
            );
            break;
        }
    }
    Custom::Rectangle rec{size.width, size.height};
    draw_index += rec.draw_rectangle(target->get_world_transform(), target->get_anchor(), draw_index, {0, 0, 120}, 120);
}

void Touch_Component::update_information(Base_Node* target, void* global_data) {
    Custom::Size size_target{0, 0};
    Node_Type type = target->get_type();
    bool swallow_touches = false;
    bool is_active = false;
    switch (type) {
        case Node_Type::PROGRESSION:
        case Node_Type::BUTTON:
        case Node_Type::IMAGE_UI: {
            Image_UI_Node* image_ui = reinterpret_cast<Image_UI_Node*>(target);
            size_target = image_ui->get_renderer_size();
            swallow_touches = image_ui->is_swallow_touches();
            is_active = image_ui->is_enable_touched();
            break;
        }
        case Node_Type::ANIMATION:
        case Node_Type::IMAGE: {
            Image_Node* image = reinterpret_cast<Image_Node*>(target);
            size_target = image->get_content_size();
            swallow_touches = image->is_swallow_touches();
            is_active = image->is_enable_touched();
            break;
        }
        default: {
            throw std::runtime_error(
                std::string("Touch component error: Unsuported type of Node ") + std::to_string(type) + "!"
            );
            break;
        }
    }
    this->set_active(is_active);
    if (is_active) {
        Custom::Transform world_transform = target->get_world_transform();
        Custom::Anchor_Point anchor = target->get_anchor();
        int priority = target->get_draw_index();
        this->touch_information = {
            this->touch_information.touch_id,
            priority,
            Custom::Transformed_Rectangle{
                Custom::Rectangle{size_target.width, size_target.height}.apply(world_transform, anchor)
            },
            swallow_touches,
            is_active
        };
        Touch_System::get()->request_update_touch(this->touch_information);
    }
}

void Touch_Component::handle_task(Base_Node* target, void* global_data) {
    Touch_Detail touch_detail = Touch_System::get()->query_touch_information(this->touch_information.touch_id);
    switch (touch_detail.type) {
        case Touch_Type::END: {
            Node_Type type = target->get_type();
            switch (type) {
                case Node_Type::PROGRESSION:
                case Node_Type::BUTTON:
                case Node_Type::IMAGE_UI:
                case Node_Type::ANIMATION:
                case Node_Type::IMAGE: {
                    Image_Node* image = reinterpret_cast<Image_Node*>(target);
                    auto caller = image->get_touch_caller();
                    if (caller != nullptr) {
                        caller(touch_detail.touch_position, image, global_data);
                    }
                    break;
                }
                default: {
                    throw std::runtime_error(
                        std::string("Touch component error: Unsuported type of Node ") + std::to_string(type) + "!"
                    );
                    break;
                }
            }
            break;
        }
        default: {
            break;
        }
    }
}