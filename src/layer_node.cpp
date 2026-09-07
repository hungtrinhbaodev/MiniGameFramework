#include <actions.h>
#include <defined.h>
#include <layer_node.h>
#include <math_custom.h>
#include <utils.h>
#include <wrapper.h>

Layer_Node::Layer_Node() {}

Layer_Node::Layer_Node(Custom::Size size) {
    this->size = size;
}

Layer_Node::Layer_Node(float size_x, float size_y) {
    set_content_size(size_x, size_y);
}

Layer_Node::~Layer_Node() {
    this->focus_target = nullptr;
    this->is_set_camera_position_first_time = false;
    this->strike_duration_move = 0.f;
}

Custom::Size Layer_Node::get_content_size() {
    return this->size;
}

Node_Type Layer_Node::get_type() {
    return Node_Type::LAYER;
}

void Layer_Node::set_content_size(Custom::Size size) {
    this->size = size;
}

void Layer_Node::set_content_size(float size_x, float size_y) {
    set_content_size({size_x, size_y});
}

void Layer_Node::set_show_boundary(bool is_show_boundary) {
    this->is_show_boundary = is_show_boundary;
}

void Layer_Node::set_camera_focus_on(Base_Node* target, float strike_duration_move) {
    this->focus_target = target;
    this->strike_duration_move = strike_duration_move;
    this->schedule(
        Defined::KEY_LAYER_SCHEDULE_CAMERA_MOVE,
        this->strike_duration_move,
        [this](Base_Node* target, void* global_data) {
            if (this->is_set_camera_position_first_time && this->strike_duration_move > 0.f) {
                this->stop_action(Defined::ACTION_MOVE_CAMERA_TAG);
                this->do_action(
                    Action::sequence(
                        Action::move_to(
                            this->strike_duration_move * 1.5,
                            this->next_layer_with_camera_position,
                            Action_Ease::SINE_OUT
                        )
                    ),
                    Defined::ACTION_MOVE_CAMERA_TAG
                );
            }
        }
    );
}

void Layer_Node::set_enable_layer_color(bool enable) {
    this->enable_layer_color = enable;
}

void Layer_Node::set_layer_color(Custom::Color color) {
    this->layer_color = color;
}

void Layer_Node::set_clipping(bool is_clipping) {
    this->is_clipping = is_clipping;
}

void Layer_Node::before_draw_children(Custom::Transform& world_transform, int& draw_index) {
    if (this->is_show_boundary || Libs_Wrapper::is_debug_mode()) {
        glm::vec3 color = {255, 0, 0};
        Custom::Rectangle rec{this->size.width, this->size.height};
        draw_index += rec.draw_border_rectangle(world_transform, anchor, draw_index);
    }

    if (this->is_clipping) {
        Libs_Wrapper::start_draw_clipping(this->size.width, this->size.height, {world_transform, anchor, draw_index});
        draw_index++;
    }
}

void Layer_Node::draw(Custom::Transform& world_transform, int& draw_index) {
    if (this->enable_layer_color) {
        Libs_Wrapper::draw_rectangle(
            this->get_content_size().width,
            this->get_content_size().height,
            {world_transform, this->anchor, draw_index, this->layer_color}
        );
        draw_index++;
    }
}

void Layer_Node::after_draw_children(Custom::Transform& world_transform, int& draw_index) {
    if (this->is_clipping) {
        Libs_Wrapper::end_draw_clipping(draw_index);
        draw_index++;
    }
}

void Layer_Node::compute_world_transform(Custom::Transform& world_transform) {
    this->handle_camera_focus_on_target(world_transform);
    Node::compute_world_transform(world_transform);
}

bool Layer_Node::is_my_child(Base_Node* node) {
    for (Base_Node* child : children) {
        if (node == child)
            return true;
    }
    return false;
}

void Layer_Node::handle_camera_focus_on_target(const Custom::Transform& world_transform) {
    if (this->focus_target == nullptr)
        return;
    /**
     * Note: if the focus target is not the children of layer we ignore it
     * to descrease compute in next frame!
     */
    if (!this->is_my_child(this->focus_target)) {
        this->focus_target = nullptr;
        this->unschedule(Defined::KEY_LAYER_SCHEDULE_CAMERA_MOVE);
        return;
    }
    float screen_width = Libs_Wrapper::get_screen_width();
    float screen_height = Libs_Wrapper::get_screen_height();
    Custom::Size layer_size = this->get_content_size();
    Custom::Anchor_Point anchor = this->get_anchor();
    glm::vec2 layer_anchor_position = glm::vec2{anchor.x, anchor.y} * glm::vec2{layer_size.width, layer_size.height};
    glm::vec2 target_position = this->focus_target->get_position() + layer_anchor_position;
    Custom::Rectangle target_rect = {screen_width, screen_height};
    glm::vec2 bottom_left_camera_position =
        target_position - (glm::vec2{screen_width, screen_height} * glm::vec2{0.5, 0.5});
    Custom::Rectangle_Area target_area = {
        bottom_left_camera_position.x, bottom_left_camera_position.y, screen_width, screen_height
    };
    Custom::Rectangle_Area layer_area{0, 0, layer_size.width, layer_size.height};
    target_area.fix_with(layer_area);
    glm::vec2 layer_world_position = -glm::vec2{target_area.x, target_area.y};
    layer_world_position += layer_anchor_position;
    glm::vec2 layer_local_position = layer_world_position - world_transform.position;
    if (!this->is_set_camera_position_first_time) {
        this->set_position(layer_local_position);
        this->is_set_camera_position_first_time = true;
    }
    if (this->strike_duration_move <= 0) {
        this->set_position(layer_local_position);
    } else {
        this->next_layer_with_camera_position = layer_local_position;
    }
}
