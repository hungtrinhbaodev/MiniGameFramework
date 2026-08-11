#pragma once
#include <custom.h>
#include <image_node.h>

class Image_UI_Node : public Image_Node {
public:
    Custom::Size get_renderer_size();
    bool is_enable_nine_scale();

    void set_enable_nine_scale(bool enable_nine_scale);
    void set_cap_insets(float x, float y, float width, float height);
    void set_renderer_size(float width, float height);
    void set_renderer_size(Custom::Size size);
    void set_enable_bondary(bool enable_boundary);

protected:
    void draw(Custom::Transform& world_transform, int& draw_index) override;

private:
    bool enable_boundary = false;
    bool enable_nine_scale = false;
    Custom::Size renderer_size;
    Custom::Rectangle_Area cap_inset_area;
};