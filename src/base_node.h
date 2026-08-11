#pragma once
#include <custom.h>
#include <node_type.h>

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Base_Node {
public:
    Base_Node();
    ~Base_Node();

    float get_x();
    float get_y();
    float get_scale_x();
    float get_scale_y();
    float get_rotation();
    float get_anchor_x();
    float get_anchor_y();
    glm::vec2 get_position();
    glm::vec2 get_scale();
    Custom::Anchor_Point get_anchor();
    int get_opacity();
    int get_z_order();
    int get_tag();
    std::string get_name();
    bool is_visible();
    bool is_cascade_opacity();
    Base_Node* get_parent();
    bool is_flipped_x();
    bool is_flipped_y();
    Custom::Transform& modify_transform();
    Custom::Transform get_transform();
    virtual Node_Type get_type();

    std::vector<Base_Node*>& get_children();
    Base_Node* get_child_by_tag(int tag);
    Base_Node* get_child_by_name(std::string name);

    void set_x(float x);
    void set_y(float y);
    void set_scale_x(float scale_x);
    void set_scale_y(float scale_y);
    void set_rotation(float roation);
    void set_anchor_x(float anchor_x);
    void set_anchor_y(float anchor_y);
    void set_position(glm::vec2 position);
    void set_scale(glm::vec2 scale);
    void set_anchor(glm::vec2 anchor);
    void set_opacity(int opacity);
    void set_z_order(int z_order);
    void set_visible(bool visible);
    void set_cascade_opacity(bool cascade);
    void set_flipped_x(bool flipped_x);
    void set_flipped_y(bool flipped_y);

    void set_tag(int tag);
    void set_name(std::string name);
    void add_child(Base_Node* child);
    void travel(float delta_time);
    void remove_from_parent(bool is_cleanup = true);
    bool remove_child(Base_Node* child, bool is_cleanup = false);

protected:
    Custom::Transform transform;
    Custom::Anchor_Point anchor;
    Custom::Flipped flipped;
    bool casecade_opacity = false;
    int z_order;
    std::vector<Base_Node*> children;
    Base_Node* parent = nullptr;

    void visit(Custom::Transform& world_transform, float delta_time, int& draw_index);
    virtual void before_draw_children(Custom::Transform& world_transform, int& draw_index);
    virtual void draw(Custom::Transform& world_transform, int& draw_index);
    virtual void after_draw_children(Custom::Transform& world_transform, int& draw_index);
    virtual void update(float delta_time);

    // Call when node attach from parent
    virtual void enter();
    // Call when node detech from parent
    virtual void exit();

private:
    int tag = -1;
    std::string name = "";
    bool visible = true;
    int total_node = 1;
    std::vector<Base_Node*> cleanup_children;
    // Invalid happen when node in list cleanup_children
    // with this node we don't keep track it anymore
    bool is_valid = true;

    void sort_nodes();
    void cleanup_invalid_children();
};