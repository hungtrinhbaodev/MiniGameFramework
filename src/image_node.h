#pragma once
#include <node.h>

#include <string>

class Image_Node : public Node {
public:
    Image_Node();
    Image_Node(std::string image_path);
    ~Image_Node();

    std::string get_image();
    glm::vec2 get_content_size();

    void set_image(std::string image_path);

protected:
    void draw(Transform& world_transform);

private:
    std::string image_path = "";
};