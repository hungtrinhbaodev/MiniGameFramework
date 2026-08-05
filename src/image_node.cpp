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

Image_Node::~Image_Node() {

}

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

void Image_Node::set_image(std::string image) {
    this->image_path = image;
}

void Image_Node::draw(Transform& world_transform) {
    if (image_path == "") return;
    Libs_Wrapper::draw_image(
        image_path,
        {
            transform.position.x, transform.position.y,
            transform.scale.x, transform.scale.y, 
            transform.rotation,
            transform.z_order,
            transform.anchor.x, transform.anchor.y
        }
    );
}
