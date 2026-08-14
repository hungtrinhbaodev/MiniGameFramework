#include <defined.h>
#include <image_node.h>
#include <touch_component.h>
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

std::function<void(glm::vec2, Base_Node*, void*)> Image_Node::get_touch_caller() {
    return this->touch_caller;
}

bool Image_Node::is_enable_touched() {
    return this->touch_enabled;
}

bool Image_Node::is_swallow_touches() {
    return this->swallow_touches;
}

void Image_Node::set_image(std::string image) {
    this->image_path = image;
}

void Image_Node::set_touch_enabled(bool touch_enabled) {
    this->touch_enabled = touch_enabled;
    if (touch_enabled) {
        Base_Component* component = this->get_component_by_name(Defined::COMPONENT_TOUCH_NAME);
        if (component == nullptr) {
            component = new Touch_Component();
            component->set_name(Defined::COMPONENT_TOUCH_NAME);
            this->add_component(component);
        }
    }
}

void Image_Node::set_swallow_touches(bool swallow_touches) {
    this->swallow_touches = swallow_touches;
}

void Image_Node::set_touched_caller(std::function<void(glm::vec2, Base_Node*, void*)> caller) {
    this->touch_caller = caller;
}

void Image_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    Node::draw(world_transform, draw_index);
    if (image_path == "")
        return;
    if (Libs_Wrapper::is_debug_mode()) {
        Custom::Size size = get_content_size();
        Custom::Rectangle rec{size.width, size.height};
        draw_index += rec.draw_border_rectangle(world_transform, this->anchor, draw_index, {255, 0, 255});
    }
    Libs_Wrapper::draw_image(image_path, {world_transform, this->anchor, draw_index, {255, 255, 255}});
    draw_index++;
}
