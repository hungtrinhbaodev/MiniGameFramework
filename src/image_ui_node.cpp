#include <image_ui_node.h>
#include <wrapper.h>

int to_row(int index) {
    return index / 3;
}

int to_col(int index) {
    return index % 3;
}

int to_index(int row, int col) {
    return row * 3 + col;
};

Custom::Distance_1D find_distance_x(
    int row, const Custom::Rectangle_Area& origin_area, const Custom::Rectangle_Area& cap_inset_area
) {
    int cap_inset_x = std::max(origin_area.x, cap_inset_area.x);
    int cap_inset_width = std::min(origin_area.width, cap_inset_area.width);
    Custom::Distance_1D distance;
    switch (row) {
        // part at left side
        case 0: {
            distance.x = 0;
            distance.length = cap_inset_x;
            break;
        }
        // part at middle side
        case 1: {
            distance.x = cap_inset_x;
            distance.length = cap_inset_width;
            break;
        }
        // part at right side
        default: {
            distance.x = cap_inset_width + cap_inset_x;
            distance.length = origin_area.width - (cap_inset_width + cap_inset_x);
            break;
        }
    }
    return distance;
}

Custom::Distance_1D find_distance_y(
    int col, const Custom::Rectangle_Area& origin_area, const Custom::Rectangle_Area& cap_inset_area
) {
    int cap_inset_y = std::max(origin_area.y, cap_inset_area.y);
    int cap_inset_height = std::min(origin_area.height, cap_inset_area.height);
    Custom::Distance_1D distance;
    switch (col) {
        // part at bottom side
        case 0: {
            distance.x = 0;
            distance.length = cap_inset_y;
            break;
        }
        // part at middle side
        case 1: {
            distance.x = cap_inset_y;
            distance.length = cap_inset_height;
            break;
        }
        // part at top side
        default: {
            distance.x = cap_inset_height + cap_inset_y;
            distance.length = origin_area.height - (cap_inset_height + cap_inset_y);
            break;
        }
    }
    return distance;
};

void draw_parts(int i, const Custom::Draw_Attributes attribute, int& draw_index) {
    Custom::Rectangle rec = {attribute.rect_texture.width, attribute.rect_texture.height};
    Custom::Color color = {100, 255, 100};
    draw_index += rec.draw_rectangle(attribute.transform, {0.f, 0.f}, draw_index, color);
}

Custom::Size Image_UI_Node::get_renderer_size() {
    if (this->enable_nine_scale) {
        return this->renderer_size;
    }
    return get_content_size();
}

bool Image_UI_Node::is_enable_nine_scale() {
    return this->enable_nine_scale;
}

Node_Type Image_UI_Node::get_type() {
    return Node_Type::IMAGE_UI;
}

void Image_UI_Node::set_enable_nine_scale(bool enable_nine_scale) {
    this->enable_nine_scale = enable_nine_scale;
    this->set_renderer_size(this->get_content_size());
}

void Image_UI_Node::set_renderer_size(Custom::Size size) {
    this->renderer_size = size;
}

void Image_UI_Node::set_cap_insets(float x, float y, float width, float height) {
    this->cap_inset_area = {x, y, width, height};
}

void Image_UI_Node::set_renderer_size(float width, float height) {
    this->renderer_size = {width, height};
}

void Image_UI_Node::set_enable_bondary(bool enable_boundary) {
    this->enable_boundary = enable_boundary;
}

void Image_UI_Node::set_force_renderer_color(Custom::Color color) {
    this->force_renderer_color = color;
}

void Image_UI_Node::set_enable_force_renderer_color(bool enable_force_renderer_color) {
    this->enable_force_renderer_color = enable_force_renderer_color;
}

void Image_UI_Node::draw_without_nine_scale(Custom::Transform& world_transform, int& draw_index) {
    Custom::Size size = this->get_content_size();
    if (this->enable_boundary || Libs_Wrapper::is_debug_mode()) {
        Custom::Rectangle rec = {size.width, size.height};
        Custom::Color color = {255, 0, 255};
        draw_index += rec.draw_rectangle(world_transform, this->anchor, draw_index, color);
    }
    glm::vec2 scale_renderer{1, 1};
    if (this->enable_nine_scale) {
        scale_renderer = {this->renderer_size.width / size.width, this->renderer_size.height / size.height};
    }
    world_transform.scale *= scale_renderer;
    Libs_Wrapper::draw_image(
        this->get_image(),
        {world_transform, this->anchor, draw_index},
        this->enable_force_renderer_color,
        this->force_renderer_color
    );
    world_transform.scale /= scale_renderer;
    draw_index++;
}

void Image_UI_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    Custom::Size size = this->get_content_size();
    if (!this->enable_nine_scale) {
        this->draw_without_nine_scale(world_transform, draw_index);
        return;
    }
    Custom::Rectangle_Area origin_area = {0, 0, size.width, size.height};
    std::array<Custom::Size, 9> size_at_parts;
    std::array<Custom::Draw_Attributes, 9> nine_draw_parts;
    for (int i = 0; i < nine_draw_parts.size(); i++) {
        int row = to_row(i);
        int col = to_col(i);
        Custom::Distance_1D distance_x = find_distance_x(row, origin_area, this->cap_inset_area);
        Custom::Distance_1D distance_y = find_distance_y(col, origin_area, this->cap_inset_area);
        nine_draw_parts[i].rect_texture = {distance_x.x, distance_y.x, distance_x.length, distance_y.length};
        size_at_parts[i] = {distance_x.length, distance_y.length};
    }
    Custom::Rectangle_Area left_bottom_area = nine_draw_parts[to_index(0, 0)].rect_texture;
    Custom::Rectangle_Area right_bottom_area = nine_draw_parts[to_index(2, 0)].rect_texture;
    Custom::Rectangle_Area left_top_area = nine_draw_parts[to_index(0, 2)].rect_texture;
    float min_width = left_bottom_area.width + right_bottom_area.width;
    float min_height = left_bottom_area.height + left_top_area.height;
    if (renderer_size.width < min_width || renderer_size.height < min_height) {
        this->draw_without_nine_scale(world_transform, draw_index);
        return;
    }
    Custom::Size renderer_size = {
        std::max(min_width, this->renderer_size.width), std::max(min_height, this->renderer_size.height)
    };
    // Find local scale at each part
    for (int i = 0; i < 9; i++) {
        if (to_row(i) == 1) {
            float width_at_part = nine_draw_parts[i].rect_texture.width;
            if (width_at_part <= 0)
                continue;
            float remain_width = renderer_size.width - min_width;
            nine_draw_parts[i].transform.scale.x = width_at_part <= 0 ? 0 : remain_width / width_at_part;
            size_at_parts[i].width = remain_width;
        }
        if (to_col(i) == 1) {
            float height_at_part = nine_draw_parts[i].rect_texture.height;
            if (height_at_part <= 0)
                continue;
            float remain_height = renderer_size.height - min_height;
            nine_draw_parts[i].transform.scale.y = height_at_part <= 0 ? 0 : remain_height / height_at_part;
            size_at_parts[i].height = remain_height;
        }
    }
    glm::vec2 origin_position = {-renderer_size.width * anchor.x, -renderer_size.height * anchor.y};
    // Update local position x of all parts
    for (int col = 0; col < 3; col++) {
        if (!flipped.x) {
            for (int row = 1; row < 3; row++) {
                int last_row = row - 1;
                int last_index = to_index(last_row, col);
                Custom::Draw_Attributes& last_part = nine_draw_parts[to_index(last_row, col)];
                Custom::Draw_Attributes& current_part = nine_draw_parts[to_index(row, col)];
                Custom::Transform& last_transform = last_part.transform;
                Custom::Transform& current_transform = current_part.transform;
                current_transform.position.x = last_transform.position.x + size_at_parts[last_index].width;
            }
        } else {
            for (int row = 1; row > -1; row--) {
                int last_row = row + 1;
                int last_index = to_index(last_row, col);
                Custom::Draw_Attributes& last_part = nine_draw_parts[to_index(last_row, col)];
                Custom::Draw_Attributes& current_part = nine_draw_parts[to_index(row, col)];
                Custom::Transform& last_transform = last_part.transform;
                Custom::Transform& current_transform = current_part.transform;
                current_transform.position.x = last_transform.position.x + size_at_parts[last_index].width;
            }
        }
    }
    // Update local position y of all parts
    for (int row = 0; row < 3; row++) {
        if (!flipped.y) {
            for (int col = 1; col < 3; col++) {
                int last_col = col - 1;
                int last_index = to_index(row, last_col);
                Custom::Draw_Attributes& last_part = nine_draw_parts[to_index(row, last_col)];
                Custom::Draw_Attributes& current_part = nine_draw_parts[to_index(row, col)];
                Custom::Transform& last_transform = last_part.transform;
                Custom::Transform& current_transform = current_part.transform;
                current_transform.position.y = last_transform.position.y + size_at_parts[last_index].height;
            }
        } else {
            for (int col = 1; col > -1; col--) {
                int last_col = col + 1;
                int last_index = to_index(row, last_col);
                Custom::Draw_Attributes& last_part = nine_draw_parts[to_index(row, last_col)];
                Custom::Draw_Attributes& current_part = nine_draw_parts[to_index(row, col)];
                Custom::Transform& last_transform = last_part.transform;
                Custom::Transform& current_transform = current_part.transform;
                current_transform.position.y = last_transform.position.y + size_at_parts[last_index].height;
            }
        }
    }
    for (int i = 0; i < 9; i++) {
        nine_draw_parts[i].transform.position += origin_position;
    }
    for (int i = 0; i < 9; i++) {
        if (size_at_parts[i].width <= 0 || size_at_parts[i].height <= 0)
            continue;
        Custom::Draw_Attributes& attribute = nine_draw_parts[i];
        Custom::Transform world_transform_duplicate = world_transform;
        attribute.draw_index = draw_index;
        world_transform_duplicate.forward(attribute.transform, true, this->flipped);
        attribute.transform = world_transform_duplicate;
        attribute.is_use_rect_texture = true;
        Libs_Wrapper::draw_image(this->get_image(), attribute, enable_force_renderer_color, force_renderer_color);
        draw_index++;
        if (this->enable_boundary || Libs_Wrapper::is_debug_mode()) {
            draw_parts(i, attribute, draw_index);
        }
    }
}