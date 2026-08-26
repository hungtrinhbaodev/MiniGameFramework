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
    if (enable_draw_rect) {
        return {draw_rect.width, draw_rect.height};
    }
    Image_Info image_info = Libs_Wrapper::image_info(this->image_path, this->load_mode);
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

void Image_Node::set_image(std::string image, Defined::LOAD_MODE load_mode) {
    this->image_path = image;
    this->load_mode = load_mode;
    if (load_mode == Defined::LOAD_MODE::IMMEDIATE) {
        this->resource_loaded_caller = nullptr;
    } else {
        this->is_call_loaded_resource = false;
    }
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

void Image_Node::set_resource_loaded_caller(std::function<void()> caller) {
    this->resource_loaded_caller = caller;
}

void Image_Node::set_enable_draw_rect(bool enable_draw_rect) {
    this->enable_draw_rect = enable_draw_rect;
}

void Image_Node::set_draw_rect(Custom::Rectangle_Area draw_rect) {
    this->draw_rect = draw_rect;
}

void Image_Node::handle_personal_task(float delta_time, void* global_data) {
    Node::handle_personal_task(delta_time, global_data);
    if (this->load_mode == Defined::LOAD_MODE::ASYNC && !this->is_call_loaded_resource) {
        Image_Info texture_info = Libs_Wrapper::image_info(this->image_path);
        if (texture_info.state == Defined::RESOURCE_LOADED_STATE::LOADED) {
            if (this->resource_loaded_caller != nullptr) {
                this->resource_loaded_caller();
            }
            this->on_texture_loaded();
            this->resource_loaded_caller = nullptr;
            this->is_call_loaded_resource = true;
        }
    }
}

void Image_Node::on_texture_loaded() {}

void Image_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    Node::draw(world_transform, draw_index);
    if (image_path == "")
        return;
    if (Libs_Wrapper::is_debug_mode()) {
        Custom::Size size = get_content_size();
        Custom::Rectangle rec{size.width, size.height};
        draw_index += rec.draw_border_rectangle(world_transform, this->anchor, draw_index, {255, 0, 255});
    }
    Custom::Draw_Attributes draw_attributes = {world_transform, this->anchor, draw_index, this->get_color()};
    draw_attributes.is_use_rect_texture = enable_draw_rect;
    draw_attributes.rect_texture = draw_rect;
    Libs_Wrapper::draw_image(image_path, draw_attributes, false, Custom::Color{0, 0, 0}, load_mode);
    draw_index++;
}
