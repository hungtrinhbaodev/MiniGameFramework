#include <image_node.h>
#include <wrapper.h>

#include <iostream>

Image_Node::Image_Node() {
    transform.anchor = {0.5f, 0.5f};
}

Image_Node::Image_Node(std::string image_path) {
    transform.anchor = {0.5f, 0.5f};
    set_image(image_path);
}

Image_Node::~Image_Node() {}

std::string Image_Node::get_image() {
    return image_path;
}

glm::vec2 Image_Node::get_content_size() {
    if (image_path == "") {
        return {0, 0};
    }
    Image_Info image_info = Libs_Wrapper::image_info(image_path);
    return {image_info.width, image_info.height};
}

bool Image_Node::is_flipped_x() {
    return this->flipped_x;
}

bool Image_Node::is_flipped_y() {
    return this->flipped_y;
}

void Image_Node::set_image(std::string image) {
    this->image_path = image;
}

void Image_Node::set_flipped_x(bool flipped_x) {
    this->flipped_x = flipped_x;
}

void Image_Node::set_flipped_y(bool flipped_y) {
    this->flipped_y = flipped_y;
}

void Image_Node::draw(Transform& world_transform, int& draw_index) {
    if (image_path == "")
        return;
    Libs_Wrapper::draw_image(
        image_path,
        {world_transform.position.x,
         world_transform.position.y,
         world_transform.scale.x,
         world_transform.scale.y,
         world_transform.rotation,
         draw_index,
         transform.anchor.x,
         transform.anchor.y,
         {world_transform.color.x, world_transform.color.y, world_transform.color.z},
         world_transform.opacity,
         flipped_x,
         flipped_y}
    );
    draw_index++;
}
