#pragma once
#include <node.h>

class Layer_Node : public Node {
public:
    Layer_Node();
    Layer_Node(glm::vec2 size);
    Layer_Node(float size_x, float size_y);
    ~Layer_Node();

    void set_clipping(bool is_clipping);
    glm::vec2 get_content_size();
    void set_content_size(glm::vec2 size);
    void set_content_size(float size_x, float size_y);

protected:
    void before_draw_children(Transform& world_transform, int& draw_index) override;
    void after_draw_children(Transform& world_transform, int& draw_index) override;

private:
    bool is_clipping;
    glm::vec2 size;
};