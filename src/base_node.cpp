#include <base_node.h>
#include <math.h>

#include <algorithm>
#include <iostream>

glm::vec2 rotate(float angle, glm::vec2 target) {
    return {target.x * cos(angle) - target.y * sin(angle), target.x * sin(angle) + target.y * cos(angle)};
}

void Base_Node::Transform::forward(const Base_Node::Transform& other) {
    position += rotate(rotation, other.position * scale);
    rotation += other.rotation;
    scale.x *= other.scale.x;
    scale.y *= other.scale.y;
}

void Base_Node::Transform::inverse(const Base_Node::Transform& other) {
    rotation -= other.rotation;
    scale.x /= other.scale.x;
    scale.y /= other.scale.y;
    position -= rotate(rotation, other.position * scale);
}

bool Base_Node::Transform::operator<(const Base_Node::Transform& other) const {
    return z_order > other.z_order;
}

Base_Node::Base_Node() {
}

Base_Node::~Base_Node() {
}

void Base_Node::visit(Transform& world_transform, float delta_time) {
    if (!visible)
        return;

    // Update current node
    update(delta_time);

    // Draw current node
    if (parent != nullptr) {
        world_transform.forward(transform);
    }
    draw(world_transform);

    // Cascade attributes into it's children
    for (Base_Node* child : children) {
        child->visit(world_transform, delta_time);
    }

    // Inverse to other visit can use again
    if (parent != nullptr) {
        world_transform.inverse(transform);
    }
}

void Base_Node::draw(Transform& world_transform) {
}

void Base_Node::update(float delta_time) {
}

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

void Base_Node::add_child(Base_Node* child) {
    if (child == nullptr) {
        return;
    }
    children.push_back(child);
    std::sort(children.begin(), children.end());
    child->parent = this;
}

void Base_Node::travel(float delta_time) {
    visit(transform, delta_time);
}
