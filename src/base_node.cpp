#include <base_node.h>
#include <math_custom.h>

#include <algorithm>
#include <iostream>

void Base_Node::Transform::forward(const Base_Node::Transform& other, bool is_cascade_opacity) {
    position += Math::rotate(rotation, other.position * scale);
    rotation += other.rotation;
    scale.x *= other.scale.x;
    scale.y *= other.scale.y;
    this->opacity = is_cascade_opacity ? std::min(this->opacity, other.opacity) : other.opacity;
}

void Base_Node::Transform::inverse(
    const Base_Node::Transform& other, unsigned char inverse_opacity
) {
    rotation -= other.rotation;
    scale.x /= other.scale.x;
    scale.y /= other.scale.y;
    position -= Math::rotate(rotation, other.position * scale);
    opacity = inverse_opacity;
}

bool Base_Node::Transform::operator<(const Base_Node::Transform& other) const {
    return z_order > other.z_order;
}

Base_Node::Base_Node() {}

Base_Node::~Base_Node() {
    cleanup_invalid_children();
    for (Base_Node* child : children) {
        delete (child);
    }
}

void Base_Node::visit(Transform& world_transform, float delta_time, int& draw_index) {
    // Update current node
    update(delta_time);

    // When some update the node valid we ignore it!
    if (!this->is_valid) {
        return;
    }

    // Draw current node
    unsigned char insverse_opacity = world_transform.opacity;
    if (this->parent != nullptr) {
        world_transform.forward(transform, this->parent->is_cascade_opacity());
    }

    // Make the wrapper caller function to wrap the draw of object
    // to handle something like cliping
    this->before_draw_children(world_transform, draw_index);

    // Cascade attributes into its children
    for (Base_Node* child : this->children) {
        if (child->transform.z_order < 0) {
            child->visit(world_transform, delta_time, draw_index);
        }
    }
    if (visible) {
        draw(world_transform, draw_index);
    }
    for (Base_Node* child : this->children) {
        if (child->transform.z_order >= 0) {
            child->visit(world_transform, delta_time, draw_index);
        }
    }

    this->after_draw_children(world_transform, draw_index);

    // Inverse to other visit can use again
    if (this->parent != nullptr) {
        world_transform.inverse(transform, insverse_opacity);
    }
}

void Base_Node::before_draw_children(Transform& world_transform, int& draw_index) {}

void Base_Node::draw(Transform& world_transform, int& draw_index) {}

void Base_Node::after_draw_children(Transform& world_transform, int& draw_index) {}

void Base_Node::update(float delta_time) {
    cleanup_invalid_children();
}

void Base_Node::enter() {}

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
    return transform.anchor.x;
}

float Base_Node::get_anchor_y() {
    return transform.anchor.y;
}

glm::vec2 Base_Node::get_position() {
    return transform.position;
}

glm::vec2 Base_Node::get_scale() {
    return transform.scale;
}

glm::vec2 Base_Node::get_anchor() {
    return transform.anchor;
}

int Base_Node::get_opacity() {
    return transform.opacity;
}

int Base_Node::get_z_order() {
    return transform.z_order;
}

int Base_Node::get_tag() {
    return tag;
}

std::string Base_Node::get_name() {
    return name;
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
    transform.anchor.x = anchor_x;
}

void Base_Node::set_anchor_y(float anchor_y) {
    transform.anchor.y = anchor_y;
}

void Base_Node::set_position(glm::vec2 position) {
    transform.position = position;
}

void Base_Node::set_scale(glm::vec2 scale) {
    transform.scale = scale;
}

void Base_Node::set_anchor(glm::vec2 anchor) {
    transform.anchor = anchor;
}

void Base_Node::set_opacity(int opacity) {
    transform.opacity = opacity;
}

void Base_Node::set_z_order(int z_order) {
    transform.z_order = z_order;
    if (this->parent != nullptr) {
        this->parent->sort_nodes();
    }
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

void Base_Node::set_cascade_opacity(bool cascade) {
    this->casecade_opacity = cascade;
}

void Base_Node::add_child(Base_Node* child) {
    if (child == nullptr) {
        return;
    }
    children.push_back(child);
    total_node += child->total_node;
    child->parent = this;
    child->is_valid = true;
    child->enter();
    sort_nodes();
}

void Base_Node::travel(float delta_time) {
    int start_draw_index = 0;
    visit(transform, delta_time, start_draw_index);
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
    for (int i = 0; i < children.size(); i++) {
        if (children[i] == child) {
            is_removed = true;
            children[i] = children[children.size() - 1];
            children.pop_back();
            break;
        }
    }
    if (is_removed) {
        if (is_cleanup) {
            cleanup_children.push_back(child);
        }
        child->is_valid = false;
        child->parent = nullptr;
        child->exit();
        total_node -= child->total_node;
        sort_nodes();
        return true;
    }
    return false;
}

void Base_Node::sort_nodes() {
    std::sort(children.begin(), children.end(), [](Base_Node* a, Base_Node* b) {
        return a->transform.z_order > b->transform.z_order;
    });
}

void Base_Node::cleanup_invalid_children() {
    for (Base_Node* child : cleanup_children) {
        delete (child);
    }
    cleanup_children.clear();
}
