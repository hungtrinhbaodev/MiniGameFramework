#pragma once
#include <node.h>

#include <array>

class Layer_Node : public Node {
public:
    Layer_Node();
    Layer_Node(Custom::Size size);
    Layer_Node(float size_x, float size_y);
    ~Layer_Node();

    Custom::Size get_content_size();

    void set_clipping(bool is_clipping);
    void set_content_size(Custom::Size size);
    void set_content_size(float size_x, float size_y);
    void set_show_boundary(bool is_show_boundary);

protected:
    void before_draw_children(Custom::Transform& world_transform, int& draw_index) override;
    void after_draw_children(Custom::Transform& world_transform, int& draw_index) override;

private:
    bool is_show_boundary = false;
    bool is_clipping;
    Custom::Size size;
};