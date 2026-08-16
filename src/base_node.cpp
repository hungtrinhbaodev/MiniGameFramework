#include <base_node.h>
#include <math_custom.h>
#include <utils.h>

#include <algorithm>
#include <iostream>

Base_Node::Base_Node() {}

Base_Node::~Base_Node() {
    Utils::clean_transform_origin(this);
    for (Base_Node* child : children) {
        delete (child);
    }
}

void Base_Node::visit_handle_personal_task(float delta_time, void* global_data) {
    this->added_waiting_children(delta_time, global_data);
    this->handle_personal_task(delta_time, global_data);

    if (!this->is_valid) {
        return;
    }

    for (Base_Node* child : children) {
        child->visit_handle_personal_task(delta_time, global_data);
    }
}

void Base_Node::visit_draw(Custom::Transform& world_transform, float delta_time, int& draw_index) {
    // When some update the node valid we ignore it!
    if (!this->is_valid) {
        return;
    }

    // Update current node
    update(delta_time);

    // Draw current node
    unsigned char insverse_opacity = world_transform.opacity;
    if (this->parent != nullptr) {
        world_transform.forward(transform, this->parent->is_cascade_opacity(), this->flipped);
    }

    // Don't update draw to node and its children when it invisible
    if (!this->visible) {
        // Save one world transform to use latter
        this->set_world_transform_information(world_transform, draw_index);

        world_transform.inverse(transform, insverse_opacity, this->parent->flipped);
        return;
    }

    // Sort node to draw one by one
    sort_nodes();

    // Make the wrapper caller function to wrap the draw of object
    // to handle something like cliping
    this->before_draw_children(world_transform, draw_index);

    // Cascade attributes into its children
    for (Base_Node* child : this->children) {
        if (child->z_order < 0) {
            child->visit_draw(world_transform, delta_time, draw_index);
        }
    }

    draw(world_transform, draw_index);

    // Save one world transform to use latter
    this->set_world_transform_information(world_transform, draw_index);

    for (Base_Node* child : this->children) {
        if (child->z_order >= 0) {
            child->visit_draw(world_transform, delta_time, draw_index);
        }
    }

    // End wrapper caller
    this->after_draw_children(world_transform, draw_index);

    // Inverse to other visit can use again
    if (this->parent != nullptr) {
        world_transform.inverse(transform, insverse_opacity, this->parent->flipped);
    }
}

void Base_Node::visit_cleanup_invalid_children(float delta_time, void* global_data) {
    this->cleanup_invalid_children();
    for (Base_Node* child : children) {
        child->visit_cleanup_invalid_children(delta_time, global_data);
    }
}

void Base_Node::set_world_transform_information(Custom::Transform world_transform, int draw_index) {
    this->world_transform = world_transform;
    this->draw_index = draw_index;
    this->update_world_transform_information(this->world_transform, draw_index);
}

void Base_Node::handle_personal_task(float delta_time, void* global_data) {}

void Base_Node::update_world_transform_information(Custom::Transform& world_transform, int draw_index) {}

void Base_Node::before_draw_children(Custom::Transform& world_transform, int& draw_index) {}

void Base_Node::draw(Custom::Transform& world_transform, int& draw_index) {}

void Base_Node::after_draw_children(Custom::Transform& world_transform, int& draw_index) {}

void Base_Node::update(float delta_time) {}

void Base_Node::enter(void* global_data) {}

void Base_Node::exit() {}

float Base_Node::get_x() {
    return transform.position.x;
}

float Base_Node::get_y() {
    return transform.position.y;
}

float Base_Node::get_scale_x() {
    return transform.scale.x;
}

float Base_Node::get_scale_y() {
    return transform.scale.y;
}

float Base_Node::get_rotation() {
    return transform.rotation;
}

float Base_Node::get_anchor_x() {
    return this->anchor.x;
}

float Base_Node::get_anchor_y() {
    return this->anchor.y;
}

glm::vec2 Base_Node::get_position() {
    return transform.position;
}

glm::vec2 Base_Node::get_scale() {
    return transform.scale;
}

Custom::Anchor_Point Base_Node::get_anchor() {
    return this->anchor;
}

int Base_Node::get_opacity() {
    return transform.opacity;
}

int Base_Node::get_z_order() {
    return z_order;
}

int Base_Node::get_tag() {
    return tag;
}

std::string Base_Node::get_name() {
    return name;
}

Custom::Color Base_Node::get_color() {
    return transform.color;
}

bool Base_Node::is_visible() {
    return visible;
}

bool Base_Node::is_cascade_opacity() {
    return casecade_opacity;
}

Base_Node* Base_Node::get_parent() {
    return parent;
}

bool Base_Node::is_flipped_x() {
    return this->flipped.x;
}

bool Base_Node::is_flipped_y() {
    return this->flipped.y;
}

int Base_Node::get_draw_index() {
    return this->draw_index;
}

void* Base_Node::get_user_data(std::string key) {
    if (this->user_data.find(key) == this->user_data.end()) {
        return nullptr;
    }
    return this->user_data[key];
}

std::vector<Base_Node*>& Base_Node::get_children() {
    return children;
}

Base_Node* Base_Node::get_child_by_tag(int tag) {
    for (Base_Node* child : children) {
        if (child->tag == tag) {
            return child;
        }
    }
    return nullptr;
}

Base_Node* Base_Node::get_child_by_name(std::string name) {
    for (Base_Node* child : children) {
        if (child->name == name) {
            return child;
        }
    }
    return nullptr;
}

Custom::Transform& Base_Node::modify_transform() {
    return this->transform;
}

Custom::Transform Base_Node::get_transform() {
    return this->transform;
}

Custom::Transform Base_Node::get_world_transform() {
    return this->world_transform;
}

Node_Type Base_Node::get_type() {
    return Node_Type::BASE_NODE;
}

void Base_Node::set_x(float x) {
    transform.position.x = x;
}

void Base_Node::set_y(float y) {
    transform.position.y = y;
}

void Base_Node::set_scale_x(float scale_x) {
    transform.scale.x = scale_x;
}

void Base_Node::set_scale_y(float scale_y) {
    transform.scale.y = scale_y;
}

void Base_Node::set_rotation(float rotation) {
    transform.rotation = rotation;
}

void Base_Node::set_anchor_x(float anchor_x) {
    this->anchor.x = anchor_x;
}

void Base_Node::set_anchor_y(float anchor_y) {
    this->anchor.y = anchor_y;
}

void Base_Node::set_position(glm::vec2 position) {
    transform.position = position;
}

void Base_Node::set_scale(glm::vec2 scale) {
    transform.scale = scale;
}

void Base_Node::set_anchor(glm::vec2 anchor) {
    this->anchor = {anchor.x, anchor.y};
}

void Base_Node::set_opacity(int opacity) {
    transform.opacity = opacity;
}

void Base_Node::set_z_order(int z_order) {
    this->z_order = z_order;
}

void Base_Node::set_tag(int tag) {
    this->tag = tag;
}

void Base_Node::set_visible(bool visible) {
    this->visible = visible;
}

void Base_Node::set_name(std::string name) {
    this->name = name;
}

void Base_Node::set_color(Custom::Color color) {
    this->transform.color = color;
}

void Base_Node::set_user_data(std::string key, void* data) {
    if (this->user_data.find(key) != this->user_data.end()) {
        std::cout << "Node warrning key is duplicate, please check it again!" << std::endl;
    }
    this->user_data[key] = data;
}

void Base_Node::set_cascade_opacity(bool cascade) {
    this->casecade_opacity = cascade;
}

void Base_Node::set_flipped_x(bool flipped_x) {
    this->flipped.x = flipped_x;
}

void Base_Node::set_flipped_y(bool flipped_y) {
    this->flipped.y = flipped_y;
}

void Base_Node::add_child(Base_Node* child) {
    if (child == nullptr) {
        return;
    }
    child->parent = this;
    waiting_added_children.push_back(child);
}

void Base_Node::travel(float delta_time, void* global_data) {
    // Handle personal task of each node before draw
    visit_handle_personal_task(delta_time, global_data);
    // Loop all node to draw into scene
    int start_draw_index = 0;
    Custom::Transform world_transform = transform;
    visit_draw(world_transform, delta_time, start_draw_index);
    visit_cleanup_invalid_children(delta_time, global_data);
}

void Base_Node::remove_from_parent(bool is_cleanup) {
    if (this->parent == nullptr) {
        return;
    }
    this->parent->remove_child(this, is_cleanup);
}

bool Base_Node::remove_child(Base_Node* child, bool is_cleanup) {
    if (child == nullptr) {
        return false;
    }
    bool is_removed = false;
    int remove_index = -1;
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            is_removed = true;
            remove_index = i;
            break;
        }
    }
    if (is_removed) {
        // children.erase(children.begin() + remove_index);
        child->is_cleanup = is_cleanup;
        child->is_valid = false;
        cleanup_children.push_back(child);
        return true;
    }
    return false;
}

void Base_Node::sort_nodes() {
    std::sort(children.begin(), children.end(), [](Base_Node* a, Base_Node* b) { return a->z_order < b->z_order; });
}

void Base_Node::cleanup_invalid_children() {
    for (int i = 0; i < children.size(); i++) {
        Base_Node* child = children[i];
        if (!child->is_valid) {
            children[i] = children.back();
            child->exit();
            child->parent = nullptr;
            if (child->is_cleanup) {
                delete (child);
            }
            i--;
            children.pop_back();
        }
    }
}

void Base_Node::added_waiting_children(float delta_time, void* global_data) {
    for (Base_Node* child : waiting_added_children) {
        child->is_valid = true;
        child->is_cleanup = false;
        children.push_back(child);
        child->enter(global_data);
    }
    waiting_added_children.clear();
}
