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

    bool is_flipped_x();
    bool is_flipped_y();

    void set_image(std::string image_path);
    void set_flipped_x(bool flipped_x);
    void set_flipped_y(bool flipped_y);

protected:
    void draw(Custom::Transform& world_transform, int& draw_index) override;

private:
    std::string image_path = "";
    Custom::Flipped flipped;
    bool flipped_x = false;
    bool flipped_y = false;
};