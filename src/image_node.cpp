#include <image_node.h>
#include <wrapper.h>

#include <iostream>

Image_Node::Image_Node() {
    this->anchor = {0.5f, 0.5f};
}

Image_Node::Image_Node(std::string image_path) {
    this->anchor = {0.5f, 0.5f};
    set_image(image_path);
}

Image_Node::~Image_Node() {}

std::string Image_Node::get_image() {
    return image_path;
}

Custom::Size Image_Node::get_content_size() {
    if (image_path == "") {
        return {0, 0};
    }
    Image_Info image_info = Libs_Wrapper::image_info(image_path);
    return image_info.size;
}

Node_Type Image_Node::get_type() {
    return Node_Type::IMAGE;
}

void Image_Node::set_image(std::string image) {
    this->image_path = image;
}

void Image_Node::set_touch_enabled(bool touch_enabled) {
    touch_info.is_listening = touch_enabled;
    if (touch_enabled) {
        if (this->touch_component_id == -1) {
            this->touch_component_id = Touch_System::get()->request_touch_listenner();
        }
    }
    Touch_System::get()->request_update_touch(this->touch_component_id, touch_info);
}

void Image_Node::set_swallow_touches(bool swallow_touches) {
    touch_info.swallow_touches = swallow_touches;
    Touch_System::get()->request_update_touch(this->touch_component_id, touch_info);
}

void Image_Node::set_touched_caller(std::function<void(glm::vec2, Base_Node*)> caller) {
    this->caller = caller;
}

void Image_Node::handle_personal_task() {
    if (touch_info.is_listening) {
        Touch_Detail touch_detail = Touch_System::get()->query_touch_information(touch_component_id);
        switch (touch_detail.type) {
            case Touch_Type::BEGIN: {
                if (caller != nullptr) {
                    caller(touch_detail.touch_position, this);
                }
                break;
            }
        }
    }
}

void Image_Node::update_world_transform_information(Custom::Transform& world_transform, int draw_index) {
    if (touch_info.is_listening) {
        Custom::Size size = get_touch_size();
        Custom::Rectangle rec{size.width, size.height};
        touch_info.touch_area.points = rec.apply(world_transform, this->anchor);
        touch_info.priority = draw_index;
        Touch_System::get()->request_update_touch(touch_component_id, touch_info);
    }
}

void Image_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    if (image_path == "")
        return;
    if (Libs_Wrapper::is_debug_mode()) {
        Custom::Size size = get_content_size();
        Custom::Rectangle rec{size.width, size.height};
        draw_index += rec.draw_rectangle(world_transform, this->anchor, draw_index, {255, 0, 255});
    }
    Libs_Wrapper::draw_image(image_path, {world_transform, anchor, draw_index, {255, 255, 255}});
    draw_index++;
}

Custom::Size Image_Node::get_touch_size() {
    return get_content_size();
}
