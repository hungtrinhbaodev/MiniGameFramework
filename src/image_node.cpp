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

bool Image_Node::is_flipped_x() {
    return this->flipped.x;
}

bool Image_Node::is_flipped_y() {
    return this->flipped.y;
}

void Image_Node::set_image(std::string image) {
    this->image_path = image;
}

void Image_Node::set_flipped_x(bool flipped_x) {
    this->flipped.x = flipped_x;
}

void Image_Node::set_flipped_y(bool flipped_y) {
    this->flipped.y = flipped_y;
}

void Image_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    if (image_path == "")
        return;
    Libs_Wrapper::draw_image(image_path, {world_transform, anchor, draw_index, {255, 255, 255}, flipped});
    draw_index++;
}
