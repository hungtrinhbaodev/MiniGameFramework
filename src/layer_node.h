#pragma once
#include <node.h>

#include <array>

class Layer_Node : public Node {
public:
    struct Rectangle {
        float width;
        float height;
        std::array<glm::vec2, 4> apply(Base_Node::Transform& transform, glm::vec2 anchor);
        int draw_rectangle(
            Base_Node::Transform& transform,
            glm::vec2 anchor,
            int base_draw_index = 0,
            glm::vec3 color = {255.f, 0.f, 0.f},
            float thin = 3.f
        );
    };

    Layer_Node();
    Layer_Node(glm::vec2 size);
    Layer_Node(float size_x, float size_y);
    ~Layer_Node();

    void set_clipping(bool is_clipping);
    glm::vec2 get_content_size();
    void set_content_size(glm::vec2 size);
    void set_content_size(float size_x, float size_y);
    void set_show_boundary(bool is_show_boundary);

protected:
    void before_draw_children(Transform& world_transform, int& draw_index) override;
    void after_draw_children(Transform& world_transform, int& draw_index) override;

private:
    bool is_show_boundary = false;
    bool is_clipping;
    glm::vec2 size;
};