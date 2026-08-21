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
    Node_Type get_type() override;

    void set_clipping(bool is_clipping);
    void set_content_size(Custom::Size size);
    void set_content_size(float size_x, float size_y);
    void set_show_boundary(bool is_show_boundary);
    void set_camera_focus_on(Base_Node* child, float strike_duration_move = 0.f);

protected:
    void before_draw_children(Custom::Transform& world_transform, int& draw_index) override;
    void after_draw_children(Custom::Transform& world_transform, int& draw_index) override;
    void compute_world_transform(Custom::Transform& world_transform) override;

private:
    bool is_show_boundary = false;
    bool is_clipping = false;
    Custom::Size size;

    Base_Node* focus_target = nullptr;
    float strike_duration_move = 0.f;
    glm::vec2 next_layer_with_camera_position{0, 0};
    bool is_set_camera_position_first_time = false;
    bool is_my_child(Base_Node* node);
    void handle_camera_focus_on_target(const Custom::Transform& world_transform);
};