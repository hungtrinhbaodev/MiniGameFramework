#pragma once
#include <node.h>

#include <string>

class Image_Node : public Node {
public:
    Image_Node();
    Image_Node(std::string image_path);
    ~Image_Node();

    std::string get_image();
    Custom::Size get_content_size();
    Node_Type get_type() override;

    void set_image(std::string image_path);

protected:
    void draw(Custom::Transform& world_transform, int& draw_index) override;

private:
    std::string image_path = "";
};